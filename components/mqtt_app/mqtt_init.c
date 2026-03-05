#include "mqtt_manager.h"
#include "mqtt_subscriber.h"

QueueHandle_t mqtt_subscription_queue = NULL;

void mqtt_init(void) {
  mqtt_subscription_queue = xQueueCreate(10, sizeof(mqtt_received_data_t));

  xTaskCreate(mqtt_management_task, "mqtt_management_task", 4096, NULL, 5, NULL);
  xTaskCreate(mqtt_subscription_handler_task, "mqtt_subscription_handler_task", 4096, NULL, 5, NULL);
  
}