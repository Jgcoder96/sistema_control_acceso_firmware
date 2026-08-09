#ifndef NVS_OFFLINE_SYNC_TASK_H
#define NVS_OFFLINE_SYNC_TASK_H

/**
 * @file nvs_offline_sync_task.h
 * @brief Tarea de vaciado de eventos offline a través de Mesh.
 */

/**
 * @brief Extrae eventos de la NVS y los envía cuando vuelve la conexión.
 *
 * Se despierta periódicamente o es disparada manualmente, comprueba
 * si hay logs pendientes y los bombea hacia el nodo Root (o hacia MQTT).
 *
 * @par Ejemplo de uso:
 * @code
 * // En la inicialización:
 * xTaskCreate(offline_sync_task, "offline_sync_task", ...);
 * @endcode
 */
void offline_sync_task(void *pvParameters);

#endif 