#ifndef WIFI_MESH_TASKS_H
#define WIFI_MESH_TASKS_H

#include <stdbool.h> 
#include "esp_mesh.h"
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "wifi_mesh_config.h"         

/**
 * @file wifi_mesh_tasks.h
 * @brief Tareas de transmisión, recepción y procesamiento.
 */

extern QueueHandle_t mesh_rx_queue; 
extern QueueHandle_t mesh_tx_queue; 

/**
 * @brief Escucha a la red Mesh (esp_mesh_recv) y mete a la cola RX.
 */
void mesh_receiver_task(void *arg);

/**
 * @brief Toma de la cola TX y envía a la red Mesh (esp_mesh_send).
 */
void mesh_transmitter_task(void *arg);

/**
 * @brief Saca de la cola RX y rutea el mensaje a la lógica de negocio.
 */
void mesh_processor_task(void *arg);

#endif 