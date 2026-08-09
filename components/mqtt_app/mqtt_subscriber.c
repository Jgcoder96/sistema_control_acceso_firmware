/**
 * @file mqtt_subscriber.c
 * @brief Implementación de la suscripción y recepción de eventos MQTT.
 */
#include "cJSON.h"
#include "esp_log.h"
#include <string.h> 

#include "mqtt_config.h"
#include "mqtt_manager.h"
#include "mqtt_subscription_manager.h"
#include "wifi_mesh_tasks.h"
#include "wifi_mesh_transmission.h"

static const char *TAG = "MQTT_SUBSCRIBER";

extern QueueHandle_t mqtt_subscription_queue;

void mqtt_subscription_handler_task(void *pvParameters) {
   mqtt_received_data_t *received_ptr;

  while (1) {
    // Esperar indefinidamente (portMAX_DELAY) hasta que llegue un mensaje a la cola
    if (xQueueReceive(mqtt_subscription_queue, &received_ptr, portMAX_DELAY)) {  

      // Intentar decodificar el payload asumiendo que es un JSON válido
      cJSON *root = cJSON_Parse(received_ptr->data);

      if (root != NULL) {
        // En caso de recibir un array de JSON, usar el primer elemento
        cJSON *actual_root = cJSON_IsArray(root) ? cJSON_GetArrayItem(root, 0) : root;

        // Rutear a la función manejadora dependiendo del tópico en el que se recibió
        if (strcmp(received_ptr->topic, SUBSCRIBE_IN_TOPIC_SYNC_RESPONSE) == 0) subscription_manager_sync_device(actual_root);
        else if (strcmp(received_ptr->topic, SUBSCRIBE_IN_TOPIC_SYNC_TRIGGER) == 0) subscription_manager_sync_trigger(actual_root);
        
        cJSON_Delete(root);
      } else {
          ESP_LOGE(TAG, "[MQTT] Error JSON: %s", cJSON_GetErrorPtr());
      }
        
      // Liberar siempre la memoria reservada por malloc en el gestor de eventos
      free(received_ptr->data);
      free(received_ptr);
    }
  }
}