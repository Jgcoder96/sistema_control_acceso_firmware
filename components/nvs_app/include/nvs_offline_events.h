#ifndef NVS_OFFLINE_EVENTS_H
#define NVS_OFFLINE_EVENTS_H

#include "esp_err.h"
#include "app_types.h"

/**
 * @file nvs_offline_events.h
 * @brief Almacenamiento circular de accesos cuando no hay conexión (Offline).
 */

/// Límite máximo de eventos almacenables en la cola NVS
#define MAX_OFFLINE_LOGS 100 

/**
 * @brief Inicializa los punteros (head, tail, count) del arreglo circular en NVS.
 */
esp_err_t init_offline_storage(void);

/**
 * @brief Guarda una lectura de acceso en memoria no volátil de forma circular.
 *
 * Si la cola (MAX_OFFLINE_LOGS) está llena, sobrescribe el registro más antiguo (tail).
 *
 * @par Ejemplo de uso:
 * @code
 * // En la tarea Wiegand o RFID al no haber conexión:
 * access_event_t ev;
 * ev.card_id = 123456;
 * store_event_offline(&ev);
 * @endcode
 */
esp_err_t store_event_offline(access_event_t *event);

/**
 * @brief Extrae y elimina el registro de acceso más antiguo (FIFO).
 *
 * @param out_event Puntero donde se depositará la estructura de datos extraída.
 */
esp_err_t pop_offline_event(access_event_t *out_event);

/**
 * @brief Obtiene el número total de eventos encolados en NVS.
 */
uint16_t get_offline_event_count(void);

#endif 