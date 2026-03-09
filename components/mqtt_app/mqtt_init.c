#include "mqtt_init.h"
#include "mqtt_manager.h"
#include "mqtt_subscriber.h"

// static const char *TAG = "MQTT_INIT";

QueueHandle_t mqtt_subscription_queue = NULL;

void mqtt_init(void) {
  mqtt_subscription_queue = xQueueCreate(MQTT_SUBSCRIPTION_QUEUE_LENGTH, sizeof(mqtt_received_data_t));

  xTaskCreate(mqtt_management_task, "mqtt_management_task", MQTT_MANAGEMENT_TASK_SIZE, NULL, MQTT_MANAGEMENT_TASK_PRIO, NULL);
  xTaskCreate(mqtt_subscription_handler_task, "mqtt_subscription_handler_task", MQTT_SUBSCRIPTION_HANDLER_TASK_SIZE, NULL, MQTT_SUBSCRIPTION_HANDLER_TASK_PRIO, NULL);
  
}