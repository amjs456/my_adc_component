#include "freertos/idf_additions.h"
#include "my_adc_component.h"

#include "esp_err.h"
#include "freertos/FreeRTOS.h"


void app_main(void) {
    ESP_ERROR_CHECK(my_adc_init());
    xTaskCreate(my_adc_read_task, "my_adc_read_task", 4096, NULL, 5, NULL);
}
