#ifndef RTC_SYNC_H
#define RTC_SYNC_H

#include "esp_err.h"
#include <stdint.h>

/**
 * @file sync_rtc.h
 * @brief Sincronización del tiempo interno y hardware RTC.
 */

/**
 * @brief Ajusta el reloj del ESP32 y el módulo físico RTC.
 *
 * Recibe un Timestamp en UTC, actualiza la hora del sistema operativo 
 * (gettimeofday) y escribe los registros I2C del módulo (DS3231).
 *
 * @par Ejemplo de uso:
 * @code
 * uint32_t current_utc = 1718302390;
 * sync_rtc(current_utc);
 * @endcode
 *
 * @param timestamp_utc Tiempo actual en formato epoch (segundos).
 * @return ESP_OK si se configuró correctamente en hardware.
 */
esp_err_t sync_rtc(uint32_t timestamp_utc);

#endif