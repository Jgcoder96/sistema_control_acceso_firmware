#ifndef RTC_GET_TIME_FROM_MODULE_H
#define RTC_GET_TIME_FROM_MODULE_H

/**
 * @file rtc_get_time_from_module.h
 * @brief Lectura de diagnóstico de hora desde el módulo I2C.
 */

/**
 * @brief Extrae la hora del DS3231/DS1307 y la imprime.
 *
 * Exclusivamente para depuración (debug). Convierte la hora BCD a decimal
 * y la muestra por log. No altera el sistema operativo.
 *
 * @par Ejemplo de uso:
 * @code
 * rtc_get_time_from_module(); // Imprime: [RTC] 15/06/2024 10:30:00
 * @endcode
 */
void rtc_get_time_from_module(void);

#endif 