#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "mqtt_client.h"
#include <inttypes.h>    
#include <string.h>     
#include <time.h>   

#include "esp_log.h"     // Añadido para logs
#include "nvs.h"         // Añadido para NVS

#include "app_types.h"
#include "wiegand_types.h"
#include "wifi_mesh_transmission.h"
#include "wifi_mesh_info.h"

static const char *TAG = "WIEGAND_PROCESS_DATA_TASK";

static void validar_acceso_local(uint64_t full_id) {
  nvs_handle_t handle;
  char key[16];

  uint32_t card_id_32 = (uint32_t)full_id;

  snprintf(key, sizeof(key), "%lu", card_id_32);

  ESP_LOGI(TAG, "🔍 Buscando tarjeta en NVS. Key generada: [%s]", key);

  if (nvs_open("storage", NVS_READONLY, &handle) != ESP_OK) {
    return;
  }

  size_t size;
    
  esp_err_t err = nvs_get_blob(handle, key, NULL, &size);

  if (err == ESP_OK) {
    ESP_LOGI(TAG, "✅ ¡ENCONTRADA! Acceso local permitido para: %s", key);
  } else {
      if (err == ESP_ERR_NVS_NOT_FOUND) {
            ESP_LOGW(TAG, "❌ NO ENCONTRADA. La llave [%s] no existe en 'storage'", key);
      } else {
            ESP_LOGE(TAG, "Error NVS: %s", esp_err_to_name(err));
      }
  }

  nvs_close(handle);
}

extern QueueHandle_t wiegand_reader_queue;

void wiegand_process_data_task(void *pvParameters) {
  wiegand_card_t card;

  while (1) {    
    if (xQueueReceive(wiegand_reader_queue, &card, portMAX_DELAY) == pdTRUE) {
      validar_acceso_local(card.full_id);
      app_packet_t data = {0};
      memcpy(data.source_mac, node_mesh_info.mac, 6);
      data.msg_type = MSG_TYPE_CARD;
      data.payload.access_event.timestamp = (uint32_t)time(NULL);
      
      snprintf(data.payload.access_event.card_id, 
               sizeof(data.payload.access_event.card_id), 
               "%" PRIu64, 
               card.full_id);

      send_upstream(&data);
    }
  }
}