/**
 * @file wiegand_monitor_task.c
 * @brief Implementación del monitoreo de bits recibidos.
 */
#include "esp_log.h"

#include "wiegand_internal.h"
#include "wiegand_types.h"

static const char *TAG = "WIEGAND_MONITOR_TASK";

extern QueueHandle_t wiegand_reader_queue;

void wiegand_monitor_task(void *pvParameters) {
  wiegand_card_t cardData;
    
  while (1) {
    uint64_t now = esp_timer_get_time();

    // Si tenemos bits y han pasado más de 50ms (50000us) desde el último,
    // asumimos que la transmisión física terminó.
    if (g_wiegand_ctx->bit_count > 0 && (now - g_wiegand_ctx->last_bit_time_us) > 50000) {
      uint64_t raw; 
      int count;

      // Entrar a sección crítica para sacar los datos sin que una interrupción nos pise
      portENTER_CRITICAL(&g_wiegand_ctx->mux);
      raw = g_wiegand_ctx->bit_buffer; 
      count = g_wiegand_ctx->bit_count;
      g_wiegand_ctx->bit_buffer = 0; 
      g_wiegand_ctx->bit_count = 0;
      portEXIT_CRITICAL(&g_wiegand_ctx->mux);

      cardData.raw_bits = raw;
      cardData.bit_count = count;
            
      // Decodificación básica (Ej. W26)
      if (count == 26) {
        cardData.format = WIEGAND_FORMAT_W26;
        // W26: 1 bit paridad par, 8 bits FC, 16 bits ID, 1 bit paridad impar
        cardData.full_id = (raw >> 1) & 0xFFFFFF;      
        cardData.facility_code = (raw >> 17) & 0xFF;   
        cardData.card_id = (raw >> 1) & 0xFFFF;
      } else {
        cardData.format = WIEGAND_FORMAT_UNKNOWN;
        cardData.full_id = raw;
      }

      // Enviar la tarjeta armada a la cola de procesamiento
      if (xQueueSend(wiegand_reader_queue, &cardData, 0) != pdPASS) {
        ESP_LOGW(TAG, "[WIEGAND] Cola llena, tarjeta descartada.");
      }
    }
    vTaskDelay(pdMS_TO_TICKS(20));
  }
}