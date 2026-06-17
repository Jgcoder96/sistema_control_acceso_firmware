#include "nvs_offline_sync_task.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include <string.h>

// Librerías de tu proyecto
#include "app_types.h"
#include "nvs_offline_events.h"
#include "wifi_mesh_info.h"
#include "wifi_mesh_transmission.h"

static const char *TAG = "SYNC_TASK";

extern node_mesh_info_t node_mesh_info; 

void offline_sync_task(void *pvParameters) {
  ESP_LOGI(TAG, "Tarea de sincronización offline iniciada.");
    
  while (1) {
    if (node_mesh_info.is_mqtt_connected && node_mesh_info.is_mesh_connected) {        
      uint16_t pending = get_offline_event_count();  
      if (pending > 0) {
        ESP_LOGI(TAG, "Conexión detectada. Sincronizando %u eventos...", pending);
        while (get_offline_event_count() > 0 && node_mesh_info.is_mqtt_connected) {
          access_event_t event;
          if (pop_offline_event(&event) == ESP_OK) {
            app_packet_t packet;
            memset(&packet, 0, sizeof(packet));
                        
            memcpy(packet.source_mac, node_mesh_info.mac, 6);
            packet.msg_type = MSG_TYPE_CARD;
            packet.payload.access_event = event;

            send_upstream(&packet);
                        
            ESP_LOGI(TAG, "Evento offline enviado.");
 
            vTaskDelay(pdMS_TO_TICKS(200));
          }
        }
        ESP_LOGI(TAG, "Sincronización finalizada.");
      }
    }    
    vTaskDelay(pdMS_TO_TICKS(10000));
  }
}