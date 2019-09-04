#include "driver/gpio.h"
#include "esp_sleep.h"

static bool gpio_interrupt_enable = false;
static bool gpio_interrupt_rising = false;

void esp32renard_gpio_interrupt_enable(bool rising)
{
	gpio_interrupt_enable = true;
	gpio_interrupt_rising = rising;
}

void esp32renard_gpio_interrupt_disable(void)
{
	gpio_interrupt_enable = false;
}

void esp32renard_gpio_interrupt_continue(void)
{
	if (gpio_interrupt_enable) {
		gpio_wakeup_enable(CONFIG_RENARD_S2LP_INT_GPIO,
				gpio_interrupt_rising ? GPIO_INTR_HIGH_LEVEL : GPIO_INTR_LOW_LEVEL);
		esp_sleep_enable_gpio_wakeup();
	}
}
