#ifndef WIFI_MESH_EVENT_HANDLER_H
#define WIFI_MESH_EVENT_HANDLER_H

#include "esp_event.h"
#include "esp_netif.h"

/**
 * @file wifi_mesh_event_handler.h
 * @brief Escucha de eventos de red e IP (conexión/desconexión).
 */

extern esp_netif_t *netif_sta;

/**
 * @brief Captura los cambios de estado del WiFi y del Mesh.
 *
 * Determina si el nodo ha sido promovido a ROOT, si ha recibido una 
 * IP del router, o si la conexión se ha caído, actualizando el contexto global.
 */
void wifi_mesh_event_handler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data);

#endif