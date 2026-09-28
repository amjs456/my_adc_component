#include "my_adc_component.h"

#include "driver/i2s_common.h"
#include "driver/i2s_types.h"
#include "esp_check.h"
#include "esp_err.h"

#include "driver/i2s_std.h"
#include "esp_log.h"
#include "hal/i2s_types.h"
#include "freertos/FreeRTOS.h"
#include "sdkconfig.h"
#include <stddef.h>
#include <stdint.h>

#define BUF_SIZE 64

static char* TAG = "my-adc";



static i2s_chan_handle_t rx_chan;

esp_err_t my_adc_init() {
    esp_err_t err;

    i2s_chan_config_t rx_chan_cfg = I2S_CHANNEL_DEFAULT_CONFIG(I2S_NUM_0, I2S_ROLE_MASTER);
    err = i2s_new_channel(&rx_chan_cfg, NULL, &rx_chan);
    if (err != ESP_OK){
        ESP_LOGE(TAG, "i2s_new_channel failed");
        return err;
    }

    i2s_std_config_t rx_std_cfg = {
        .clk_cfg = I2S_STD_CLK_DEFAULT_CONFIG(48000),
        .slot_cfg = I2S_STD_MSB_SLOT_DEFAULT_CONFIG(I2S_DATA_BIT_WIDTH_32BIT, I2S_SLOT_MODE_STEREO),
        .gpio_cfg = {
            .mclk = CONFIG_I2S_MCLK_GPIO,
            .bclk = CONFIG_I2S_BCLK_GPIO,
            .ws = CONFIG_I2S_WS_GPIO,
            .din = CONFIG_I2S_DIN_GPIO,
            .dout = I2S_GPIO_UNUSED,
            .invert_flags = {
                .mclk_inv = false,
                .bclk_inv = false,
                .ws_inv = false,
            }
        }
    };
    err = i2s_channel_init_std_mode(rx_chan, &rx_std_cfg);
    if(err != ESP_OK){
        ESP_LOGE(TAG, "i2s_channel_init_std_mode failed");
        return err;
    }
    return ESP_OK;
}

void my_adc_read_task(void *args){
    uint8_t *r_buf = (uint8_t *)calloc(1, BUF_SIZE);
    assert(r_buf);
    size_t r_bytes = 0;


    ESP_ERROR_CHECK(i2s_channel_enable(rx_chan));

    while(1) {
        if(i2s_channel_read(rx_chan, r_buf, BUF_SIZE, &r_bytes, portMAX_DELAY) == ESP_OK) {
            printf("[0] %x [1] %x [2] %x [3] %x\n[4] %x [5] %x [6] %x [7] %x\n\n",
                   r_buf[0], r_buf[1], r_buf[2], r_buf[3], r_buf[4], r_buf[5], r_buf[6], r_buf[7]);
        } else {
            printf("Read task: i2s read failed\n");
        }
    }
}

