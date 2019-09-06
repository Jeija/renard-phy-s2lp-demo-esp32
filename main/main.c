#include <stdio.h>
#include <string.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_sleep.h"
#include "nvs_flash.h"
#include "nvs.h"

#include "renard_phy_s2lp_protocol.h"
#include "renard_phy_s2lp.h"

/* ***PUT YOUR SIGFOX CREDENTIALS HERE*** */
uint8_t key[] = {0x47, 0x9e, 0x44, 0x80, 0xfd, 0x75, 0x96, 0xd4, 0x5b, 0x01, 0x22, 0xfd, 0x28, 0x2d, 0xb3, 0xcf};
uint32_t devid = 0x004d33db;
uint8_t payload[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x12, 0x34, 0x56};

static uint16_t next_seqnum(void)
{
    /* Increment sequence number: Load current value from NVS, increment and store updated value */
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ESP_ERROR_CHECK(nvs_flash_init());
    } else {
		ESP_ERROR_CHECK(err);
	}

	nvs_handle_t nvshandle;
	ESP_ERROR_CHECK(nvs_open("sigfox", NVS_READWRITE, &nvshandle));
	uint16_t seqnum = 0;
	nvs_get_u16(nvshandle, "seqnum", &seqnum);
	seqnum = (seqnum + 1) % 0xfff;
	nvs_set_u16(nvshandle, "seqnum", seqnum);

	return seqnum;
}

void app_main(void)
{
	/* S2-LP SPI communication initialization */
	printf("[renard-phy-s2lp-demo-esp32] Initializing S2-LP...\r\n");

	if (renard_phy_s2lp_init()) {
		printf("[renard-phy-s2lp-demo-esp32] Initialization OK, S2-LP detected!\r\n");
	} else {
		printf("[renard-phy-s2lp-demo-esp32] Initialization FAIL - S2-LP not responding!\r\n");
		vTaskDelay(1000 / portTICK_PERIOD_MS);
		esp_restart();
	}
	renard_phy_s2lp_protocol_init(esp_random(), UL_DATARATE_100BPS);

	vTaskDelay(100 / portTICK_PERIOD_MS);

	/* Prepare uplink */
	printf("[renard-phy-s2lp-demo-esp32] Starting message transfer!\r\n");
	sfx_commoninfo common;
	common.seqnum = next_seqnum();
	common.devid = devid;
	memcpy(common.key, key, 16);

	sfx_ul_plain uplink;
	memcpy(uplink.payload, payload, sizeof(payload));
	uplink.payloadlen = sizeof(payload);
	uplink.request_downlink = true;
	uplink.singlebit = false;
	uplink.replicas = true;

	/* Actual protocol operation */
	sfx_dl_plain downlink;
	int16_t downlink_rssi;
	int err = renard_phy_s2lp_protocol_transfer(&common, &uplink, &downlink, &downlink_rssi);

	/* Log result */
	if (err == PROTOCOL_ERROR_NONE) {
		if (uplink.request_downlink) {
			printf("[renard-phy-s2lp-demo-esp32] Downlink received!\r\n");
			printf("[renard-phy-s2lp-demo-esp32] Downlink RSSI    : %d\r\n", downlink_rssi);
			printf("[renard-phy-s2lp-demo-esp32] Downlink payload : ");
			for (int i = 0; i < SFX_DL_PAYLOADLEN; i++)
				printf("%02x", downlink.payload[i]);
			printf("\r\n");
			printf("[renard-phy-s2lp-demo-esp32] Downlink CRC OK  : %d\r\n", downlink.crc_ok);
			printf("[renard-phy-s2lp-demo-esp32] Downlink MAC OK  : %d\r\n", downlink.mac_ok);
			printf("[renard-phy-s2lp-demo-esp32] Downlink FEC Use : %d\r\n", downlink.fec_corrected);
		} else {
			printf("[renard-phy-s2lp-demo-esp32] Uplink transmitted, no downlink requested.");
		}
	} else if (err == PROTOCOL_ERROR_TIMEOUT) {
		printf("[renard-phy-s2lp-demo-esp32] Timeout while waiting for downlink\r\n");
	} else {
		printf("[renard-phy-s2lp-demo-esp32] Unknown protocol error occurred\r\n");
	}

	/* Reboot countdown */
	for (int i = 3; i >= 0; i--) {
		printf("Restarting in %d seconds...\n", i);
		vTaskDelay(1000 / portTICK_PERIOD_MS);
	}
	printf("Restarting now.\n");
	fflush(stdout);
	esp_restart();
}

