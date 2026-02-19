#include "esp_log.h"

#include "mqtt_config.h"
#include "mqtt_manager.h"

static const char *TAG = "MQTT_SUBSCRIBER";

extern QueueHandle_t mqtt_subscription_queue;

void mqtt_subscription_handler_task(void *pvParameters) {
  mqtt_received_data_t mqtt_received_data;
  while (1) {
    if (xQueueReceive(mqtt_subscription_queue, &mqtt_received_data, portMAX_DELAY)) {
      if (strcmp(mqtt_received_data.topic, SUBSCRIBE_IN_TOPIC_CONFIG_DEVICE) == 0) {
        if (strcmp(mqtt_received_data.data, "encender") == 0) {
          ESP_LOGI(TAG, "MQTT COMANDO: Encendiendo LED");        
        } else if (strcmp(mqtt_received_data.data, "apagar") == 0) {
          ESP_LOGI(TAG, "MQTT COMANDO: Apagando LED");
        }
      } else if (strcmp(mqtt_received_data.topic, SUBSCRIBE_IN_TOPIC_CONFIG_ALL_DEVICE) == 0) {
        if (strcmp(mqtt_received_data.data, "reboot") == 0) {
          ESP_LOGW(TAG, "MQTT COMANDO: Reiniciando...");
          vTaskDelay(pdMS_TO_TICKS(2000));
          esp_restart();
        }
      }
    }
  }
}