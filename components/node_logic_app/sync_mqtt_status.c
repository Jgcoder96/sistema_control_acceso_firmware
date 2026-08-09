/**
 * @file sync_mqtt_status.c
 * @brief Implementación de la actualización local del estado MQTT.
 */
#include "esp_log.h"
#include "wifi_mesh_info.h" 
#include "nvs_init.h"

static const char *TAG = "SYNC_MQTT_STATUS";

void sync_mqtt_status(bool status) {
  // Actualizar la variable global accesible por otras tareas
  node_mesh_info.is_mqtt_connected = status;
  
  // Avisar a la memoria NVS por si hay lecturas de tarjetas encoladas (offline)
  // que ahora deban ser enviadas porque el MQTT acaba de conectarse
  nvs_sync_trigger();

  ESP_LOGI(TAG, "[NODE] MQTT Sync | Estado sincronizado: %s", status ? "INICIADO" : "DETENIDO");
}