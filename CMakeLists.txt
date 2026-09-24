idf_component_register(SRCS "my_adc_component.c"
                    INCLUDE_DIRS "include"
                    PRIV_REQUIRES esp_driver_i2s esp_driver_gpio )
