#include "esp_err.h"
#include <stdint.h>

#define RINGBUFFER_SIZE 2048

esp_err_t my_adc_init();
void my_adc_read_task(void *args);