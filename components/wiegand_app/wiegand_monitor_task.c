#include "esp_log.h"

#include "wiegand_internal.h"
#include "wiegand_types.h"

static const char *TAG = "WIEGAND_MONITOR_TASK";

extern QueueHandle_t wiegand_reader_queue;

void wiegand_monitor_task(void *pvParameters) {
  wiegand_card_t cardData;
    
  while (1) {
    uint64_t now = esp_timer_get_time();

    if (g_wiegand_ctx->bit_count > 0 && (now - g_wiegand_ctx->last_bit_time_us) > 50000) {
      uint64_t raw; 
      int count;

      portENTER_CRITICAL(&g_wiegand_ctx->mux);
      raw = g_wiegand_ctx->bit_buffer; 
      count = g_wiegand_ctx->bit_count;
      g_wiegand_ctx->bit_buffer = 0; 
      g_wiegand_ctx->bit_count = 0;
      portEXIT_CRITICAL(&g_wiegand_ctx->mux);

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