#ifndef _ESP32RENARD_GPIO_H
#define _ESP32RENARD_GPIO_H

void esp32renard_gpio_interrupt_enable(bool rising);
void esp32renard_gpio_interrupt_disable(void);
void esp32renard_gpio_interrupt_continue(void);

#endif
