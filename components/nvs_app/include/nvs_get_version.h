#ifndef NVS_GET_VERSION_H
#define NVS_GET_VERSION_H

#include "esp_err.h"

/**
 * @file nvs_get_version.h
 * @brief Extracción de la versión de base de datos actual.
 */

/**
 * @brief Lee desde la NVS la versión local sincronizada.
 *
 * Busca en la partición "storage" la clave "sync_ver". Útil para
 * enviar en los heartbeats (pings) a ver si estamos desactualizados.
 *
 * @par Ejemplo de uso:
 * @code
 * char ver[16];
 * if (nvs_get_version(ver, sizeof(ver)) == ESP_OK) {
 *    printf("Versión actual: %s\n", ver);
 * }
 * @endcode
 *
 * @param version_out Puntero al buffer donde se copiará el string.
 * @param max_len Tamaño máximo del buffer.
 * @return ESP_OK si la llave existe.
 */
esp_err_t nvs_get_version(char* version_out, size_t max_len);

#endif