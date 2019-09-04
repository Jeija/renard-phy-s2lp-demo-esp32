#include <stdio.h>
#include <string.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_spi_flash.h"
#include "esp_sleep.h"

void renard_phy_s2lp_hal_init(void);
void renard_phy_s2lp_hal_spi(uint8_t length, uint8_t *in, uint8_t *out);
void renard_phy_s2lp_hal_shutdown(bool shutdown);
void renard_phy_s2lp_hal_interrupt_timeout(uint32_t milliseconds);
void renard_phy_s2lp_hal_interrupt_gpio(bool risingTrigger);
void renard_phy_s2lp_hal_interrupt_clear(void);
int renard_phy_s2lp_hal_interrupt_wait(void);


void app_main(void)
{
	printf("Hello world!\n");

	printf("Initializing SPI!\n");

	renard_phy_s2lp_hal_init();

	renard_phy_s2lp_hal_shutdown(true);
	vTaskDelay(20 / portTICK_PERIOD_MS);
	renard_phy_s2lp_hal_shutdown(false);
	vTaskDelay(20 / portTICK_PERIOD_MS);

	uint8_t txdata[] = {0x01, 0xf0, 0x00, 0x00};
	uint8_t rxdata[sizeof(txdata)];
	renard_phy_s2lp_hal_spi(4, txdata, rxdata);

	printf("RX Data: ");
	for (uint8_t i = 0; i < sizeof(rxdata); i++)
		printf("%02x", rxdata[i]);
	printf("\n");

	/* wait for gpio interrupt */
	printf("entering light sleep\n");
	fflush(stdout);
	renard_phy_s2lp_hal_interrupt_gpio(true);
	renard_phy_s2lp_hal_interrupt_timeout(10000);

	while (renard_phy_s2lp_hal_interrupt_wait()) {
		printf("woke up\n");
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

