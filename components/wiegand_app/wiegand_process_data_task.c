#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "mqtt_client.h"
#include <inttypes.h>    
#include <string.h>     
#include <time.h>   

#include "esp_log.h"     
#include "nvs.h"       

#include "app_types.h"
#include "wiegand_types.h"
#include "wifi_mesh_transmission.h"
#include "wifi_mesh_info.h"
#include "rgb_led.h"

static const char *TAG = "WIEGAND_PROCESS_DATA_TASK";

static bool validate_local_access(uint64_t full_id) {
  nvs_handle_t handle;
  char key[16];
  bool permitted = false;
  size_t size; 

  uint32_t card_id_32 = (uint32_t)full_id;
  snprintf(key, sizeof(key), "%lu", card_id_32);

  esp_err_t err = nvs_open("storage", NVS_READONLY, &handle);
  if (err != ESP_OK) {
    ESP_LOGE(TAG, "Error abriendo NVS: %s", esp_err_to_name(err));
    return false;
  }

  err = nvs_get_blob(handle, key, NULL, &size); 
  permitted = (err == ESP_OK);
  nvs_close(handle);

  if (permitted) {
    ESP_LOGI(TAG, "✅ Acceso permitido para ID: %s", key);
    rgb_led_set_color(0, 255, 0); 
  } else {
    if (err == ESP_ERR_NVS_NOT_FOUND) {
      ESP_LOGW(TAG, "❌ Tarjeta no registrada: %s", key);
    } else {
      ESP_LOGE(TAG, "❌ Error al buscar en NVS: %s", esp_err_to_name(err));
    }
    rgb_led_set_color(255, 0, 0); 
  }

  vTaskDelay(pdMS_TO_TICKS(2000));
  rgb_led_clear();

  return permitted;
}

extern QueueHandle_t wiegand_reader_queue;

void wiegand_process_data_task(void *pvParameters) {
  wiegand_card_t card;
    
  while (1) {
    if (xQueueReceive(wiegand_reader_queue, &card, portMAX_DELAY) == pdTRUE) {
      validate_local_access(card.full_id);

      static app_packet_t data;
      memset(&data, 0, sizeof(data));
      memcpy(data.source_mac, node_mesh_info.mac, 6);
      data.msg_type = MSG_TYPE_CARD;
      data.payload.access_event.timestamp = (uint32_t)time(NULL);
      snprintf(data.payload.access_event.card_id, sizeof(data.payload.access_event.card_id), "%" PRIu64, card.full_id);

      send_upstream(&data);
      ESP_LOGI(TAG, "Evento Mesh enviado.");
    }
  }
}