/**
 * @file mqtt_status_broadcast_task.c
 * @brief Implementación del sistema de broadcasting del estado MQTT.
 */
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include <string.h>

#include "app_types.h" 
#include "wifi_mesh_transmission.h"
#include "wifi_mesh_info.h"
#include "mqtt_manager.h"

extern bool mqtt_is_started; 

static const char *TAG = "MQTT_STATUS_BROADCAST";

void send_mqtt_status_update(void) {
  app_packet_t packet;
  memset(&packet, 0, sizeof(app_packet_t));

  packet.type = ROOT_TO_ALL_CHILDREN;
  packet.msg_type = MSG_TYPE_MQTT_STATUS;
  packet.payload.mqtt_status_event.mqtt_status = mqtt_is_started;

  ESP_LOGI(TAG, "[MQTT] Enviando actualización de estado MQTT: %s", mqtt_is_started ? "CONECTADO" : "DESCONECTADO");
             
  // Distribuir el mensaje a toda la sub-red (broadcast)
  broadcast_to_mesh(&packet);
}

void mqtt_status_broadcast_task(void *pvParameters) {
  while (1) {
    // Solo el Root debe emitir este broadcast, ya que es el único que
    // gestiona la conexión real con el broker MQTT
    if (node_mesh_info.is_root == true) {
      send_mqtt_status_update();
    }
    vTaskDelay(pdMS_TO_TICKS(30000));
  }
}
