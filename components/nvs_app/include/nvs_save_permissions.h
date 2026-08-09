#ifndef NVS_SAVE_PERMISSIONS_H
#define NVS_SAVE_PERMISSIONS_H

#include "app_types.h"  

/**
 * @file nvs_save_permissions.h
 * @brief Almacenamiento de permisos de usuarios (paginado).
 */

/**
 * @brief Procesa un bloque de datos (festivos y tarjetas) del backend.
 *
 * Si es la primera página, borra todo lo anterior. Luego va escribiendo 
 * cada tarjeta (y sus reglas) como un binario (blob) en la NVS. 
 * Si falta una página, pide la siguiente.
 *
 * @par Ejemplo de uso:
 * @code
 * // Al recibir un evento de sincronización desde MQTT o Mesh:
 * nvs_save_permissions(&sync_event);
 * @endcode
 * 
 * @param sync_ev Estructura con la página actual, datos y versión.
 */
void nvs_save_permissions(const sync_data_event_t *sync_ev);

#endif