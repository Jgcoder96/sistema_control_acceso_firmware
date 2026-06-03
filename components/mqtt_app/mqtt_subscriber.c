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
    if (xQueueReceive(mqtt_subscription_queue, &received_ptr, portMAX_DELAY)) {  

      cJSON *root = cJSON_Parse(received_ptr->data);

      if (root != NULL) {
        cJSON *actual_root = cJSON_IsArray(root) ? cJSON_GetArrayItem(root, 0) : root;

        if (strcmp(received_ptr->topic, SUBSCRIBE_IN_TOPIC_SYNC_RESPONSE) == 0) {
          procesar_mensaje_sincronizacion(actual_root);
        } else if (strcmp(received_ptr->topic, SUBSCRIBE_IN_TOPIC_SYNC_TRIGGER) == 0) {
          subscription_manager_sync_trigger(actual_root);
        }

        cJSON_Delete(root);
      } else {
          ESP_LOGE("SUB", "Error JSON: %s", cJSON_GetErrorPtr());
      }
        
      free(received_ptr->data);
      free(received_ptr);
    }
  }
}