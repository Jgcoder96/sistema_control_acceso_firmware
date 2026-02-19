#include "esp_log.h"
#include "esp_mac.h"

#include "wifi_mesh_tasks.h"

static const char *TAG = "WIFI_MESH_EVENT_HANDLER";

bool is_internet_available = false;
bool is_node_root = false; 

esp_netif_t *netif_sta = NULL;

void wifi_mesh_event_handler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data) {
    
  if (event_base == MESH_EVENT) {
    mesh_event_info_t *info = (mesh_event_info_t *)event_data;

    switch (event_id) {
      case MESH_EVENT_PARENT_CONNECTED:
        is_mesh_connected = true;
        my_mesh_layer = info->connected.self_layer;

        is_node_root = esp_mesh_is_root();

        if (is_node_root) {
          ESP_LOGI(TAG, "Dispositivo conectado como nodo raíz");
          if (netif_sta) esp_netif_dhcpc_start(netif_sta);
        } else {
          ESP_LOGI(TAG, "Dispositivo conectado como nodo intermedio");
          is_internet_available = false; 
        }
        break;

      case MESH_EVENT_PARENT_DISCONNECTED:
        is_mesh_connected = false;
        my_mesh_layer = -1;
        is_internet_available = false;
        ESP_LOGW(TAG, "Se perdió la conexión. Iniciando proceso de re-escaneo...");
        break;

      default:
        break;
    }
  } else if (event_base == IP_EVENT) {
      switch (event_id) {
        case IP_EVENT_STA_GOT_IP: 
          ip_event_got_ip_t *event = (ip_event_got_ip_t *)event_data;

          is_internet_available = true;
          ESP_LOGI(TAG, "Conectividad IP establecida. IP asignada por Router: " IPSTR, IP2STR(&event->ip_info.ip));
          break;
          
        default:
          break;
      }
  }
}