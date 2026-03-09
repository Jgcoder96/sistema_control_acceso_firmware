#ifndef WIFI_MESH_TASKS_H
#define WIFI_MESH_TASKS_H

#include <stdbool.h> 
#include "esp_mesh.h"
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "wifi_mesh_config.h"

extern bool is_mesh_connected;      
extern int my_mesh_layer;          
extern uint8_t my_mac[6]; 

extern QueueHandle_t mesh_rx_queue; 
extern QueueHandle_t mesh_tx_queue; 

void mesh_receiver_task(void *arg);
void mesh_transmitter_task(void *arg);
void mesh_processor_task(void *arg);

#endif 