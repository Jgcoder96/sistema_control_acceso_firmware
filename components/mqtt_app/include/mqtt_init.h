#ifndef MQTT_INIT_H
#define MQTT_INIT_H

/**
 * @file mqtt_init.h
 * @brief Inicialización de las tareas MQTT.
 */

/// Capacidad de la cola de recepción MQTT (mensajes entrantes)
#define MQTT_SUBSCRIPTION_QUEUE_LENGTH      10

/// Prioridad de la tarea que conecta/desconecta el cliente MQTT
#define MQTT_MANAGEMENT_TASK_PRIO           5
/// Stack de la tarea que gestiona la conexión MQTT
#define MQTT_MANAGEMENT_TASK_SIZE           8192 

/// Prioridad de la tarea que parsea los JSON entrantes
#define MQTT_SUBSCRIPTION_HANDLER_TASK_PRIO 5
/// Stack de la tarea de procesamiento de suscripciones
#define MQTT_SUBSCRIPTION_HANDLER_TASK_SIZE 8192

/// Prioridad de la tarea de broadcast de estado hacia los hijos
#define MQTT_STATUS_BROADCAST_TASK_PRIO 5
/// Stack de la tarea de broadcast de estado
#define MQTT_STATUS_BROADCAST_TASK_SIZE 4096

/**
 * @brief Crea las colas y lanza todas las tareas del módulo MQTT.
 *
 * Configura la infraestructura base para que el nodo pueda enviar lecturas
 * y recibir configuraciones (permisos, festivos) si asume el rol de Root.
 *
 * @par Ejemplo de uso:
 * @code
 * void app_main(void) {
 *     // Inicializar NVS, Mesh, etc...
 *     mqtt_init();
 * }
 * @endcode
 */
void mqtt_init(void);

#endif