#include "cJSON.h"
#include "esp_log.h"
#include <string.h> 

#include "wifi_mesh_tasks.h"
#include "wifi_mesh_transmission.h"
#include "nodeLogic.h"

static const char *TAG = "DEVICE_CONFIG";

void subscription_manager_configure_device(cJSON *root) {
  cJSON *mac = cJSON_GetObjectItemCaseSensitive(root, "mac");
  cJSON *config = cJSON_GetObjectItemCaseSensitive(root, "config");
  cJSON *accion = (config != NULL) ? cJSON_GetObjectItemCaseSensitive(config, "accion") : NULL;

  if (cJSON_IsString(mac) && cJSON_IsString(accion)) {
    uint8_t temp_dest_mac[6];
    
    int mac_bytes = sscanf(mac->valuestring, "%02hhx:%02hhx:%02hhx:%02hhx:%02hhx:%02hhx", 
                        &temp_dest_mac[0], &temp_dest_mac[1], 
                        &temp_dest_mac[2], &temp_dest_mac[3], 
                        &temp_dest_mac[4], &temp_dest_mac[5]);

    if (mac_bytes != 6) {
        ESP_LOGE(TAG, "Formato de MAC inválido");
        return;
    }
    app_packet_t app_packet;    
    app_packet.layer = 1; 
        
    memcpy(app_packet.source_mac, my_mac, 6);      
    memcpy(app_packet.destination_mac, temp_dest_mac, 6);

    memset(app_packet.payload.accion, 0, sizeof(app_packet.payload.accion));
    strncpy(app_packet.payload.accion, accion->valuestring, sizeof(app_packet.payload.accion) - 1);

    if (memcmp(temp_dest_mac, my_mac, 6) == 0) {
      handle_root_to_child(&app_packet, my_mac);
    } else {
      send_downstream(&app_packet);
    }
    
  } else {
    ESP_LOGW(TAG, "Error al configurar el dispositivo: datos incompletos");
  }
}

void subscription_manager_configure_all_devices(cJSON *root) {
  cJSON *config = cJSON_GetObjectItemCaseSensitive(root, "config");
  cJSON *accion = (config != NULL) ? cJSON_GetObjectItemCaseSensitive(config, "accion") : NULL;

  if (cJSON_IsString(accion)) {
    app_packet_t app_packet;
    app_packet.layer = 1;
    memcpy(app_packet.source_mac, my_mac, 6);   
        
    memset(app_packet.destination_mac, 0xFF, 6);
        
    memset(app_packet.payload.accion, 0, sizeof(app_packet.payload.accion));
    strncpy(app_packet.payload.accion, accion->valuestring, sizeof(app_packet.payload.accion) - 1);     
        
    broadcast_to_mesh(&app_packet);
    } else {
      ESP_LOGW(TAG, "Error al configurar el dispositivo");
    }
}