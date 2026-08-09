#ifndef NVS_SYNC_VERSION_H
#define NVS_SYNC_VERSION_H

/**
 * @file nvs_sync_version.h
 * @brief Máquina de estados para pedir permisos (Paginación).
 */

/**
 * @brief Tarea que negocia con el backend para bajarse la base de datos.
 *
 * Inicia solicitando la página 1, espera por el semáforo (nvs_sync_trigger)
 * y si todo sale bien, pide la 2, etc. Si hay un timeout (error), reintenta.
 *
 * @par Ejemplo de uso:
 * @code
 * xTaskCreate(nvs_sync_version, "nvs_sync_version", ...);
 * @endcode
 */
void nvs_sync_version(void *pvParameters);

#endif