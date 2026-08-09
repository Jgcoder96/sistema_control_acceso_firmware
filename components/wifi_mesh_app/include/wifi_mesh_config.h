#ifndef WIFI_MESH_CONFIG_H
#define WIFI_MESH_CONFIG_H

#include <stdint.h>

/**
 * @file wifi_mesh_config.h
 * @brief Credenciales y topología de la red WiFi Mesh.
 */

#define WIFI_SSID           "Sin Servicio"
#define WIFI_PWD            "Jesus.040620"

/// Contraseña para cifrar la comunicación interna entre nodos
#define MESH_PASSWORD       "MAPLE_MESH_123"
/// Canal de operación. 0 significa buscar dinámicamente el del router.
#define MESH_CHANNEL        0                 
/// Profundidad máxima de saltos (hops) en la malla
#define MESH_MAX_LAYER      6
/// Identificador único (MAC virtual) que define este clúster Mesh
#define MESH_ID_BYTES       { 0x77, 0x77, 0x77, 0x77, 0x77, 0x77 }

#define TX_INTERVAL_MS      10000                         

#endif