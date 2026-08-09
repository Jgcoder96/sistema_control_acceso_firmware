#ifndef MQTT_SUBSCRIPTION_MANAGER_H
#define MQTT_SUBSCRIPTION_MANAGER_H

/**
 * @file mqtt_subscription_manager.h
 * @brief Lógica de negocio para procesar los mensajes MQTT entrantes.
 */

#include "cJSON.h"

/**
 * @brief Extrae los datos de sincronización (permisos y festivos) del JSON recibido.
 *
 * Si la MAC objetivo coincide con este nodo, guarda los datos en la memoria NVS.
 * Si es para otro nodo, enruta el paquete hacia abajo por la red Mesh.
 *
 * @par Ejemplo de JSON esperado:
 * @code
 * {
 *   "mac": "base64_mac==",
 *   "version": 12,
 *   "current_page": 1,
 *   "total_pages": 1,
 *   "festivos": "base64_data==",
 *   "permisos_nvs": "base64_data=="
 * }
 * @endcode
 *
 * @param root Objeto cJSON con los datos de sincronización.
 */
void subscription_manager_sync_device(cJSON *root);

/**
 * @brief Procesa la orden manual de iniciar una sincronización.
 *
 * Obliga al nodo destino a disparar el evento de petición de datos al backend.
 *
 * @par Ejemplo de JSON esperado:
 * @code
 * {
 *   "mac": "base64_mac==",
 *   "execute": true
 * }
 * @endcode
 *
 * @param root Objeto cJSON con la MAC objetivo y bandera de ejecución.
 */
void subscription_manager_sync_trigger(cJSON *root);

#endif