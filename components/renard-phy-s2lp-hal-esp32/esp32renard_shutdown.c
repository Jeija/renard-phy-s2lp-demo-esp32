#include "driver/gpio.h"

// TODO: Everything configurable with menuconfig?
#define ESP32RENARD_SHUTDOWN_IO 16

void esp32renard_shutdown_init(void)
{
	gpio_pad_select_gpio(ESP32RENARD_SHUTDOWN_IO);
	gpio_set_direction(ESP32RENARD_SHUTDOWN_IO, GPIO_MODE_OUTPUT);
}

void esp32renard_shutdown_set(bool shutdown)
{
	gpio_set_level(ESP32RENARD_SHUTDOWN_IO, shutdown);
}
