#ifndef MQTT_MANAGER_H
#define MQTT_MANAGER_H

/**
 * @file mqtt_manager.h
 * @brief Gestión central del cliente MQTT (Conexión y Eventos).
 */

#include <stdbool.h>
#include "mqtt_client.h"

/// Longitud máxima del tópico MQTT
#define MAX_TOPIC_LEN  64
/// Longitud máxima del payload entrante
#define MAX_DATA_LEN   1024

/// Estado global de la conexión MQTT
extern bool mqtt_is_started;

/// Handler del cliente MQTT de ESP-IDF
extern esp_mqtt_client_handle_t client;

/**
 * @struct mqtt_received_data_t
 * @brief Estructura para almacenar un mensaje MQTT recibido.
 */
typedef struct {
  char topic[64];  ///< Tópico en el que se recibió el mensaje
  char *data;      ///< Puntero dinámico al contenido del payload
  int data_len;    ///< Longitud exacta del payload
} mqtt_received_data_t;

/**
 * @brief Tarea asíncrona que gestiona la conexión MQTT.
 *
 * Mantiene viva la conexión al broker únicamente si el nodo es Root
 * y cuenta con acceso a Internet. Enciende o apaga el cliente dinámicamente.
 *
 * @par Tareas internas:
 * @code
 * // Bucle infinito interno:
 * if (es_root && tiene_internet && !conectado) {
 *    esp_mqtt_client_start(client);
 * }
 * @endcode
 */
void mqtt_management_task(void *pvParameters);

#endif