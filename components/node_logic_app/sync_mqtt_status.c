#include "esp_log.h"
#include "wifi_mesh_info.h" 

static const char *TAG = "SYNC_MQTT_STATUS";

void sync_mqtt_status(bool status) {
  node_mesh_info.is_mqtt_connected = status;
    
  ESP_LOGI(TAG, "[NODE] MQTT Sync | Estado sincronizado: %s", status ? "INICIADO" : "DETENIDO");
}