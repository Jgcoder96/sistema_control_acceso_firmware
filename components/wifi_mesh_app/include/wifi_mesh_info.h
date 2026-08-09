#ifndef WIFI_MESH_INFO_H
#define WIFI_MESH_INFO_H

#include "esp_mesh.h"

/**
 * @file wifi_mesh_info.h
 * @brief Estructura de estado global de conectividad del dispositivo.
 */

/**
 * @brief Contexto en tiempo real sobre el papel de este nodo en la red.
 */
typedef struct {
  mesh_addr_t mesh_id;                ///< Identificador de la malla
  uint8_t mac[6];                     ///< Dirección MAC de este ESP32
  int layer;                          ///< Capa actual (1 = Root, >1 = Hijo)
  int rssi;                           ///< Fuerza de la señal
  bool is_root;                       ///< ¿Soy yo el nodo maestro?
  bool is_internet_available;         ///< ¿Tenemos salida al exterior?
  bool is_mesh_connected;             ///< ¿Estamos anclados a la malla?
  bool is_mqtt_connected;             ///< ¿Root conectó exitosamente al broker?
  bool is_synchronized;               ///< ¿NVS bajó toda la base de datos?
  uint16_t next_page_to_request;      ///< Paginación para Sync

} node_mesh_info_t;

/// Instancia global disponible en toda la aplicación
extern node_mesh_info_t node_mesh_info;

#endif