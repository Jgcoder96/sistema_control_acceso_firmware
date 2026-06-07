#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/rmt_tx.h"
#include "esp_log.h"
#include "rgb_led.h"

#define LED_RGB_GPIO       48   
#define RMT_RES_HZ         10000000 

static rmt_channel_handle_t led_chan = NULL;
static rmt_encoder_handle_t led_encoder = NULL;

static const rmt_symbol_word_t sym_zero = { .duration0 = 4, .level0 = 1, .duration1 = 8, .level1 = 0 };
static const rmt_symbol_word_t sym_one  = { .duration0 = 8, .level0 = 1, .duration1 = 4, .level1 = 0 };

esp_err_t rgb_led_init(void) {
  if (led_chan != NULL) return ESP_OK;

  rmt_tx_channel_config_t tx_chan_config = {
    .clk_src = RMT_CLK_SRC_DEFAULT,
    .gpio_num = LED_RGB_GPIO,
    .mem_block_symbols = 64,
    .resolution_hz = RMT_RES_HZ,
    .trans_queue_depth = 4,
  };
    
  esp_err_t ret = rmt_new_tx_channel(&tx_chan_config, &led_chan);
  if (ret != ESP_OK) return ret;

  rmt_copy_encoder_config_t copy_encoder_config = {};
  ret = rmt_new_copy_encoder(&copy_encoder_config, &led_encoder);
  if (ret != ESP_OK) return ret;

  return rmt_enable(led_chan);
}

esp_err_t rgb_led_set_color(uint8_t r, uint8_t g, uint8_t b) {
  if (led_chan == NULL || led_encoder == NULL) return ESP_ERR_INVALID_STATE;
  static rmt_symbol_word_t symbols[24];
  uint32_t color = (g << 16) | (r << 8) | b; 
  for (int i = 0; i < 24; i++) {
    symbols[i] = (color & (1 << (23 - i))) ? sym_one : sym_zero;
  }
  rmt_transmit_config_t tx_config = { .loop_count = 0 };
  return rmt_transmit(led_chan, led_encoder, symbols, sizeof(symbols), &tx_config);
}

esp_err_t rgb_led_clear(void) {
  return rgb_led_set_color(0, 0, 0);
}