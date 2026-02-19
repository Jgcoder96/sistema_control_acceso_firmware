#include "wiegand_reader.h"
#include "esp_timer.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"

static const char *TAG = "WIEGAND_DRIVER";

extern QueueHandle_t wiegand_reader_queue;

typedef struct {
  uint64_t bit_buffer;
  volatile int bit_count;
  volatile uint64_t last_bit_time_us;
  portMUX_TYPE mux;
} wiegand_context_t;

static wiegand_context_t *ctx = NULL;

static void IRAM_ATTR wiegand_isr_handler(void* arg) {
  uint32_t bit_val = (uint32_t)arg;
  portENTER_CRITICAL_ISR(&ctx->mux);
  ctx->bit_buffer = (ctx->bit_buffer << 1) | bit_val;
  ctx->bit_count++;
  ctx->last_bit_time_us = esp_timer_get_time();
  portEXIT_CRITICAL_ISR(&ctx->mux);
}

void wiegand_monitor_task(void *pvParameters) {
  wiegand_card_t cardData;
    
  while (1) {
    uint64_t now = esp_timer_get_time();

    if (ctx->bit_count > 0 && (now - ctx->last_bit_time_us) > 50000) {
      uint64_t raw; 
      int count;

      portENTER_CRITICAL(&ctx->mux);
      raw = ctx->bit_buffer; 
      count = ctx->bit_count;
      ctx->bit_buffer = 0; 
      ctx->bit_count = 0;
      portEXIT_CRITICAL(&ctx->mux);

      cardData.raw_bits = raw;
      cardData.bit_count = count;
            
      if (count == 26) {
        cardData.format = WIEGAND_FORMAT_W26;
        cardData.full_id = (raw >> 1) & 0xFFFFFF;      
        cardData.facility_code = (raw >> 17) & 0xFF;   
        cardData.card_id = (raw >> 1) & 0xFFFF;
      } else {
        cardData.format = WIEGAND_FORMAT_UNKNOWN;
        cardData.full_id = raw;
      }

      if (xQueueSend(wiegand_reader_queue, &cardData, 0) != pdPASS) {
        ESP_LOGW(TAG, "Cola llena, tarjeta descartada");
      }
    }
    vTaskDelay(pdMS_TO_TICKS(20));
  }
}

esp_err_t wiegand_init(const wiegand_config_t *config) {
  ctx = calloc(1, sizeof(wiegand_context_t));
  if (!ctx) return ESP_ERR_NO_MEM;
    
  ctx->mux = (portMUX_TYPE)portMUX_INITIALIZER_UNLOCKED;

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