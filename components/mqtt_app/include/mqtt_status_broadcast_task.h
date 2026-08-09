#ifndef MQTT_STATUS_BROADCAST_TASK_H
#define MQTT_STATUS_BROADCAST_TASK_H

/**
 * @file mqtt_status_broadcast_task.h
 * @brief Notificación del estado MQTT a través de la red Mesh.
 */

/**
 * @brief Emite un broadcast por Mesh informando si el MQTT está activo o no.
 *
 * Utilizado para que los nodos hijos sepan si el Root actual tiene conexión.
 *
 * @par Ejemplo de disparo:
 * @code
 * // Si somos Root y acabamos de conectar MQTT:
 * mqtt_is_started = true;
 * send_mqtt_status_update(); // Avisar a todos los hijos
 * @endcode
 */
void send_mqtt_status_update(void);

/**
 * @brief Tarea asíncrona que emite periódicamente el estado MQTT.
 */
void mqtt_status_broadcast_task(void *pvParameters);

#endif