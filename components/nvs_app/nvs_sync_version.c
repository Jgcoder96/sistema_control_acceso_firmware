#include "nvs_flash.h"
#include "nvs.h"
#include "esp_log.h"
#include "esp_random.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"

#include "app_types.h"
#include "mqtt_publisher.h"
#include "nvs_get_version.h"
#include "wifi_mesh_info.h"
#include "wifi_mesh_transmission.h"

static const char *TAG = "NVS_SYNC_VERSION";

extern SemaphoreHandle_t sync_trigger_sem;

#define RETRY_DELAY_MIN_MS 10000
#define RETRY_DELAY_MAX_MS 20000
#define SECURITY_TIMEOUT_MS 600000 

void nvs_sync_version(void *pvParameters) {
  node_mesh_info.next_page_to_request = 1;
  node_mesh_info.is_synchronized = false;

  static app_packet_t packet;

  while (1) {
    if (xSemaphoreTake(sync_trigger_sem, pdMS_TO_TICKS(SECURITY_TIMEOUT_MS)) == pdTRUE) {
      ESP_LOGI(TAG, "[NVS] Gatillo de sincronización activado.");
    } else {
      ESP_LOGI(TAG, "[NVS] Verificación periódica (Timeout).");
    }

    if (node_mesh_info.is_synchronized) {
        node_mesh_info.is_synchronized = false;
        node_mesh_info.next_page_to_request = 1;
    }

    
    while (!node_mesh_info.is_synchronized) {
      nvs_handle_t handle;
      uint32_t current_version = 0;
      sync_state_t current_state = SYNC_STATE_NONE;

      esp_err_t err = nvs_open("storage", NVS_READONLY, &handle);

      if (err == ESP_OK) {       
        err = nvs_get_u32(handle, "sync_ver", &current_version);
        nvs_close(handle);
        if (err == ESP_OK) current_state = SYNC_STATE_CHECK_VER;
      }

      memset(&packet, 0, sizeof(app_packet_t));

      packet.msg_type = MSG_TYPE_INITIAL_SYNC;

      memcpy(packet.source_mac, node_mesh_info.mac, 6);

      packet.payload.initial_sync_event.state = current_state;
      packet.payload.initial_sync_event.version = current_version;
      
      packet.payload.initial_sync_event.page = node_mesh_info.next_page_to_request;

      bool sent = false;

      if (node_mesh_info.is_root) {
        if (node_mesh_info.is_mqtt_connected) {
          mqtt_publisher("device/sync/request", packet);
          sent = true;
        }
      } else {
        if (node_mesh_info.is_mesh_connected) {
          packet.type = CHILD_TO_ROOT;
          send_upstream(&packet);
          sent = true;
        }
      }

      if (sent) {
        ESP_LOGI(TAG, "[NVS] Petición V.%lu -> Pág %d enviada.", current_version, node_mesh_info.next_page_to_request);
      }

      uint32_t retry_ms = RETRY_DELAY_MIN_MS + (esp_random() % (RETRY_DELAY_MAX_MS - RETRY_DELAY_MIN_MS + 1));
      
      if (xSemaphoreTake(sync_trigger_sem, pdMS_TO_TICKS(retry_ms)) == pdTRUE) {
          ESP_LOGI(TAG, "[NVS] Página recibida confirmada. Procesando siguiente paso...");
      } else {
          ESP_LOGW(TAG, "[NVS] Tiempo de espera agotado. Reintentando Pág %d...", node_mesh_info.next_page_to_request);
      }
    }

    ESP_LOGI(TAG, "[NVS] Sincronización completada. Durmiendo tarea.");
  }
}