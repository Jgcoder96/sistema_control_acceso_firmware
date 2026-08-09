#ifndef WIFI_MESH_MONITOR_RSSI_TASK_H
#define WIFI_MESH_MONITOR_RSSI_TASK_H

/**
 * @file wifi_mesh_monitor_rssi_task.h
 * @brief Monitor de potencia de señal WiFi.
 */

/**
 * @brief Tarea periódica que lee el RSSI del enlace y lo guarda en el contexto.
 *
 * @par Ejemplo de uso:
 * @code
 * xTaskCreate(monitor_rssi_task, "rssi", 3072, NULL, 5, NULL);
 * @endcode
 */
void monitor_rssi_task(void *arg);

#endif