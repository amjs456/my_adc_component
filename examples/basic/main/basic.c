#include "freertos/idf_additions.h"
#include "my_adc_component.h"

#include "esp_err.h"
#include "freertos/FreeRTOS.h"
#include "freertos/ringbuf.h"
#include "portmacro.h"
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#include "esp_spiffs.h"

#define LOG_SIZE 1024

void read_buf_task(void *args){
    RingbufHandle_t buf_handle = (RingbufHandle_t)args;
    size_t r_bytes;

    while(true){
        int32_t *samples = (int32_t *)xRingbufferReceive(buf_handle, &r_bytes, portMAX_DELAY);
        size_t n = r_bytes / sizeof(int32_t);
        for(size_t i = 0; i + 1 < n; i+=2){
            int32_t lin = samples[i] >> 8;
            int32_t rin = samples[i + 1] >> 8;
            printf("%ld\n", lin);
        }
        vRingbufferReturnItem(buf_handle, samples);
    }
}


void app_main(void) {
    RingbufHandle_t buf_handle;
    buf_handle = xRingbufferCreate(1024, RINGBUF_TYPE_NOSPLIT);
    if(buf_handle == NULL){
        printf("Failed to create ring buffer\n");
    }

    ESP_ERROR_CHECK(my_adc_init());
    xTaskCreate(my_adc_read_task, "my_adc_read_task", 4096, buf_handle, 5, NULL);
    xTaskCreate(read_buf_task ,"read_buf_task", 8192, buf_handle, 5, NULL);
}
