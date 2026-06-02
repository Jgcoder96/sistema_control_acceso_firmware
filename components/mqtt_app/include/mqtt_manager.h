#ifndef MQTT_MANAGER_H
#define MQTT_MANAGER_H

#include <stdbool.h>
#include "mqtt_client.h"

#define MAX_TOPIC_LEN  64
#define MAX_DATA_LEN   1024

extern bool mqtt_is_started;
extern esp_mqtt_client_handle_t client;

typedef struct {
  char topic[64];
  char *data;   
  int data_len;
} mqtt_received_data_t;

void mqtt_management_task(void *pvParameters);

#endif