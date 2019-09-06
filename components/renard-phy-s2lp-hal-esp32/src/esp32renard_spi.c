#include <string.h>

#include "driver/spi_master.h"

static spi_device_handle_t s2lp;

void esp32renard_spi_init(void)
{
	spi_bus_config_t buscfg = {
		.miso_io_num = CONFIG_RENARD_S2LP_MISO_GPIO,
		.mosi_io_num = CONFIG_RENARD_S2LP_MOSI_GPIO,
		.sclk_io_num = CONFIG_RENARD_S2LP_SCLK_GPIO,
		.quadwp_io_num = -1,
		.quadhd_io_num = -1,
		.max_transfer_sz = 100
	};
	ESP_ERROR_CHECK(spi_bus_initialize(VSPI_HOST, &buscfg, 1));

	spi_device_interface_config_t devcfg = {
		.command_bits = 0,
		.address_bits = 0,
		.clock_speed_hz = SPI_MASTER_FREQ_8M,
		.mode = 0,
		.spics_io_num = CONFIG_RENARD_S2LP_CS_GPIO,
		.queue_size = 1
	};
	ESP_ERROR_CHECK(spi_bus_add_device(VSPI_HOST, &devcfg, &s2lp));
}

void esp32renard_spi_raw(uint8_t length, uint8_t *in, uint8_t *out)
{
	spi_transaction_t t;
	memset(&t, 0, sizeof(t));
	t.length = (size_t)length * 8;
	t.tx_buffer = in;
	t.rx_buffer = out;

	ESP_ERROR_CHECK(spi_device_transmit(s2lp, &t));
}

