idf_component_register(SRCS "main.c"
	INCLUDE_DIRS ""
	REQUIRES soc driver spi_flash nvs_flash renard-phy-s2lp-container renard-phy-s2lp-hal-esp32
)
