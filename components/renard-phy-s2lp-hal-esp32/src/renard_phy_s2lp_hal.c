#include "esp_sleep.h"

#include "esp32renard_shutdown.h"
#include "esp32renard_timer.h"
#include "esp32renard_gpio.h"
#include "esp32renard_spi.h"

void renard_phy_s2lp_hal_init(void)
{
	esp32renard_shutdown_init();
	esp32renard_spi_init();
}

void renard_phy_s2lp_hal_spi(uint8_t length, uint8_t *in, uint8_t *out)
{
	esp32renard_spi_raw(length, in, out);
}

void renard_phy_s2lp_hal_shutdown(bool shutdown)
{
	esp32renard_shutdown_set(shutdown);
}

void renard_phy_s2lp_hal_interrupt_timeout(uint32_t milliseconds)
{
	esp32renard_timer_start(milliseconds);
}

void renard_phy_s2lp_hal_interrupt_gpio(bool risingTrigger)
{
	esp32renard_gpio_interrupt_enable(risingTrigger);
}

void renard_phy_s2lp_hal_interrupt_clear(void)
{
	esp32renard_timer_stop();
	esp32renard_gpio_interrupt_disable();
}

int renard_phy_s2lp_hal_interrupt_wait(void)
{
	esp32renard_timer_continue();
	esp32renard_gpio_interrupt_continue();

	esp_light_sleep_start();

	int cause = esp_sleep_get_wakeup_cause();
	if (cause == ESP_SLEEP_WAKEUP_TIMER)
		esp32renard_timer_stop();
	return cause == ESP_SLEEP_WAKEUP_GPIO ? true : false;
}

