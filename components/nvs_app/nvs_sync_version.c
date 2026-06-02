#include "nvs_flash.h"
#include "nvs.h"
#include "esp_log.h"
#include "esp_random.h"

#include "app_types.h"
#include "mqtt_publisher.h"
#include "nvs_get_version.h"
#include "wifi_mesh_info.h"
#include "wifi_mesh_transmission.h"

static const char *TAG = "NVS_SYNC_VERSION";

#define MIN_DELAY_MS 20000
#define MAX_DELAY_MS 40000

void nvs_sync_version(void *pvParameters) {
  char saved_version_str[32];
    
  while (1) {
    uint32_t random_ms = MIN_DELAY_MS + (esp_random() % (MAX_DELAY_MS - MIN_DELAY_MS + 1));
    vTaskDelay(pdMS_TO_TICKS(random_ms));

    if (node_mesh_info.is_synchronized) {
      ESP_LOGI(TAG, "[NVS] Nodo ya sincronizado. Finalizando tarea.");
      vTaskDelete(NULL);
      return;
    }

    uint32_t current_version = 0;
    sync_state_t current_state = SYNC_STATE_NONE;

    size_t size = sizeof(saved_version_str);
    esp_err_t err = nvs_get_version(saved_version_str, size);

    if (err == ESP_OK) {
      current_version = (uint32_t)atoi(saved_version_str);
      current_state = SYNC_STATE_CHECK_VER;
    } else {
      current_version = 0;
      current_state = SYNC_STATE_NONE;
    }

    app_packet_t packet;
    memset(&packet, 0, sizeof(app_packet_t));  
    packet.msg_type = MSG_TYPE_INITIAL_SYNC;
    memcpy(packet.source_mac, node_mesh_info.mac, 6);
                
    packet.payload.initial_sync_event.state = current_state;
    packet.payload.initial_sync_event.version = current_version;


    if (node_mesh_info.is_root) {
      if (node_mesh_info.is_mqtt_connected) {
        mqtt_publisher("device/sync", packet);

        ESP_LOGI(TAG, "[NVS] Petición de sincronización enviada (Ver: %d)", (int)current_version);
      }
    } else {
      if (node_mesh_info.is_mesh_connected && node_mesh_info.is_mqtt_connected) {
        
        send_upstream(&packet);

        ESP_LOGI(TAG, "[NVS] Petición de sincronización enviada (Ver: %d)", (int)current_version);
      }
    }
    
  }
}