#include "driver/gpio.h"

#include "wiegand_internal.h"
#include "wiegand_setup.h"

wiegand_context_t *g_wiegand_ctx = NULL;

static void IRAM_ATTR wiegand_isr_handler(void* arg) {
  if (g_wiegand_ctx == NULL) return;
  uint32_t bit_val = (uint32_t)arg;
  portENTER_CRITICAL_ISR(&g_wiegand_ctx->mux);
  g_wiegand_ctx->bit_buffer = (g_wiegand_ctx->bit_buffer << 1) | bit_val;
  g_wiegand_ctx->bit_count++;
  g_wiegand_ctx->last_bit_time_us = esp_timer_get_time();
  portEXIT_CRITICAL_ISR(&g_wiegand_ctx->mux);
} 

esp_err_t wiegand_setup(const wiegand_config_t *config) {
  g_wiegand_ctx = calloc(1, sizeof(wiegand_context_t));
  if (!g_wiegand_ctx) return ESP_ERR_NO_MEM;
    
  g_wiegand_ctx->mux = (portMUX_TYPE)portMUX_INITIALIZER_UNLOCKED;

  gpio_config_t io_conf = {
    .intr_type = GPIO_INTR_NEGEDGE,
    .mode = GPIO_MODE_INPUT,
    .pin_bit_mask = (1ULL << config->gpio_d0) | (1ULL << config->gpio_d1),
    .pull_up_en = GPIO_PULLUP_ENABLE,
  };

  gpio_config(&io_conf);
    
  gpio_install_isr_service(0); 
    
  gpio_isr_handler_add(config->gpio_d0, wiegand_isr_handler, (void*)0);
  gpio_isr_handler_add(config->gpio_d1, wiegand_isr_handler, (void*)1);

  return ESP_OK;
}