#ifndef _ESP32RENARD_SPI_H
#define _ESP32RENARD_SPI_H

void esp32renard_spi_init(void);
void esp32renard_spi_raw(uint8_t length, uint8_t *in, uint8_t *out);

#endif
