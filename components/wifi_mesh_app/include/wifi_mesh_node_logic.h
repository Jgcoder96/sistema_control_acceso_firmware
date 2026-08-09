#ifndef WIFI_MESH_NODE_LOGIC_H
#define WIFI_MESH_NODE_LOGIC_H

#include "app_types.h"

/**
 * @file wifi_mesh_node_logic.h
 * @brief Ruteo lógico de mensajes (Root vs. Hijo).
 */

/**
 * @brief Si somos Root, procesa un mensaje que subió desde un Hijo.
 */
void handle_child_to_root(const app_packet_t *msg);

/**
 * @brief Si somos Hijo, procesa un mensaje broadcast (para todos) que bajó del Root.
 */
void handle_root_to_all_children(const app_packet_t *msg);

/**
 * @brief Si somos el Hijo destino, procesa un mensaje directo que bajó del Root.
 */
void handle_root_to_child(const app_packet_t *msg, const uint8_t *my_mac);

#endif