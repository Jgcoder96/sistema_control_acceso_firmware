#ifndef WIFI_MESH_TRANSMISSION_H
#define WIFI_MESH_TRANSMISSION_H

#include "app_types.h"

/**
 * @file wifi_mesh_transmission.h
 * @brief Utilerías para encolar paquetes en la cola TX de forma semántica.
 */

/**
 * @brief (Solo Root) Envía un paquete a todos los Hijos de la malla.
 */
void broadcast_to_mesh(app_packet_t *data);

/**
 * @brief (Solo Root) Envía un paquete a un Hijo en específico.
 */
void send_downstream(app_packet_t *data);

/**
 * @brief (Solo Hijo) Envía un paquete hacia arriba, destinado al Root.
 */
void send_upstream(app_packet_t *data);

#endif