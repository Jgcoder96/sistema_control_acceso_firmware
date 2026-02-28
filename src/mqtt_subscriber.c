#include "cJSON.h"
#include "esp_log.h"
#include <string.h> 

#include "mqtt_config.h"
#include "mqtt_manager.h"
#include "wifi_mesh_tasks.h"
#include "wifi_mesh_transmission.h"
#include "mqtt_subscription_manager.h"

static const char *TAG = "MQTT_SUBSCRIBER";

extern uint8_t my_mac[6];
extern QueueHandle_t mqtt_subscription_queue;

void mqtt_subscription_handler_task(void *pvParameters) {
  mqtt_received_data_t mqtt_received_data;

  while (1) {
    if (xQueueReceive(mqtt_subscription_queue, &mqtt_received_data, portMAX_DELAY)) {  
      cJSON *root = cJSON_Parse(mqtt_received_data.data);
            
      if (root == NULL) {
        ESP_LOGE(TAG, "Error: JSON mal formado");
        continue;
      }  
    
      if (strcmp(mqtt_received_data.topic, SUBSCRIBE_IN_TOPIC_CONFIG_DEVICE) == 0) {
        subscription_manager_configure_device(root);
      } else if (strcmp(mqtt_received_data.topic, SUBSCRIBE_IN_TOPIC_CONFIG_ALL_DEVICE) == 0) {
        subscription_manager_configure_all_devices(root);
      }

      cJSON_Delete(root);
    }
  }
}