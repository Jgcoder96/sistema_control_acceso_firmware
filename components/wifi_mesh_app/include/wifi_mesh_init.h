#ifndef WIFI_MESH_INIT_H
#define WIFI_MESH_INIT_H

/**
 * @file wifi_mesh_init.h
 * @brief Punto de entrada para inicializar el stack y tareas Mesh.
 */

#define MESH_RX_QUEUE_LENGTH        10
#define MESH_TX_QUEUE_LENGTH        10

#define MESH_RECEIVER_TASK_PRIO     5
#define MESH_RECEIVER_TASK_SIZE     8192

#define MESH_TRANSMITTER_TASK_PRIO  5
#define MESH_TRANSMITTER_TASK_SIZE  8192

#define MESH_PROCESSOR_TASK_PRIO    5
#define MESH_PROCESSOR_TASK_SIZE    8192

#define MESH_MONITOR_RSSI_TASK_PRIO 5
#define MESH_MONITOR_RSSI_TASK_SIZE 3072

/**
 * @brief Inicializa las colas y dispara los hilos de red.
 *
 * Crea las colas de Transmisión (TX) y Recepción (RX). Levanta tareas 
 * independientes para enviar, escuchar y procesar la lógica de negocio.
 *
 * @par Ejemplo de uso:
 * @code
 * void app_main() {
 *    wifi_mesh_setup(); // Hardware
 *    wifi_mesh_init();  // Software
 * }
 * @endcode
 */
void wifi_mesh_init(void);

#endif