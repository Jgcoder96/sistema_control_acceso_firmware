#ifndef SYNC_MQTT_STATUS_H
#define SYNC_MQTT_STATUS_H

#include <stdbool.h>

/**
 * @file sync_mqtt_status.h
 * @brief Sincronización local del estado de conectividad MQTT.
 */

/**
 * @brief Actualiza la bandera global y avisa a la NVS.
 *
 * Guarda si el Root tiene conexión MQTT y dispara cualquier 
 * sincronización que estuviera pendiente en la memoria.
 *
 * @par Ejemplo de uso:
 * @code
 * // Al recibir un paquete MSG_TYPE_MQTT_STATUS vía Mesh:
 * sync_mqtt_status(true);
 * @endcode
 * 
 * @param status true si está conectado, false si está desconectado.
 */
void sync_mqtt_status(bool status);

#endif