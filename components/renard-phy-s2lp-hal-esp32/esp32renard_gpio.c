#include "driver/gpio.h"
#include "esp_sleep.h"

void esp32renard_gpio_interrupt(bool rising)
{
	gpio_wakeup_enable(4, rising ? GPIO_INTR_HIGH_LEVEL : GPIO_INTR_LOW_LEVEL);
	esp_sleep_enable_gpio_wakeup();
}
