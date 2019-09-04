#include "esp_sleep.h"

void esp32renard_timer_start(uint32_t milliseconds)
{
	esp_sleep_enable_timer_wakeup((uint64_t)milliseconds * 1000);
}
