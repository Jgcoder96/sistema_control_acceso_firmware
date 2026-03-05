#include "esp_log.h"

#include "convert_data_to_json.h"
#include "mqtt_manager.h"
#include "wifi_mesh_tasks.h"

static const char *TAG = "MQTT_PUBLISHER";

esp_err_t send_data_by_mqtt(const char *topic, const char *payload) {
  if (client == NULL || !mqtt_is_started) {
    ESP_LOGW(TAG, "No se puede enviar: MQTT no activo");
    return ESP_FAIL;
  }

  if (payload == NULL) return ESP_ERR_INVALID_ARG;

  int msg_id = esp_mqtt_client_publish(client, topic, payload, 0, 1, 0);
    
  if (msg_id < 0) {
    ESP_LOGE(TAG, "Error al publicar en MQTT");
    return ESP_FAIL;
  }

  return ESP_OK;
}

void mqtt_publisher(const char *topic, app_packet_t packet) {
  char *json_payload = packet_to_json(packet);
    
  if (json_payload != NULL) {
    send_data_by_mqtt(topic, json_payload);
        
    free(json_payload);
  } else {
      ESP_LOGE(TAG, "Error al crear el JSON");
  }
} 