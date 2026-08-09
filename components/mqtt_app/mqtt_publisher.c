/**
 * @file mqtt_publisher.c
 * @brief Implementación del publicador de eventos MQTT.
 */
#include "esp_log.h"

#include "convert_data_to_json.h"
#include "mqtt_manager.h"
#include "wifi_mesh_tasks.h"

static const char *TAG = "MQTT_PUBLISHER";

esp_err_t send_data_by_mqtt(const char *topic, const char *payload) {
  // Asegurarnos de que el cliente exista y esté iniciado antes de publicar
  if (client == NULL || !mqtt_is_started) {
    ESP_LOGW(TAG, "[MQTT] Error de envío: El servicio MQTT no está activo");
    return ESP_FAIL;
  }

  if (payload == NULL) return ESP_ERR_INVALID_ARG;

  // QoS 1: Garantiza que el mensaje se entregue al menos una vez
  int msg_id = esp_mqtt_client_publish(client, topic, payload, 0, 1, 0);
    
  if (msg_id < 0) {
    ESP_LOGE(TAG, "[MQTT] Error al publicar en MQTT");
    return ESP_FAIL;
  }

  return ESP_OK;
}

void mqtt_publisher(const char *topic, app_packet_t packet) {
  // Generar la cadena JSON dinámica a partir de la estructura en C
  char *json_payload = packet_to_json(packet);
    
  if (json_payload != NULL) {
    send_data_by_mqtt(topic, json_payload);
        
    // Liberar memoria para evitar fugas (memory leaks)
    free(json_payload);
  } else {
      ESP_LOGE(TAG, "[MQTT] Error al crear el JSON");
  }
}