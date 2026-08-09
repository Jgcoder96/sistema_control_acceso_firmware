#ifndef RTC_WRITE_TIME_IN_MODULE_H
#define RTC_WRITE_TIME_IN_MODULE_H

#include "driver/i2c.h"
#include <time.h> 

/**
 * @file rtc_write_time_in_module.h
 * @brief Escritura manual de hora en el módulo I2C externo.
 */

/**
 * @brief Escribe una estructura `tm` en los registros del chip RTC.
 *
 * Transforma cada campo (segundos, minutos, horas...) de decimal a BCD
 * y los envía por I2C al DS3231/DS1307.
 *
 * @par Ejemplo de uso:
 * @code
 * struct tm timeinfo;
 * // ... (llenar timeinfo) ...
 * rtc_write_time_in_module(&timeinfo);
 * @endcode
 *
 * @param t Puntero a la estructura `tm` con la fecha/hora.
 * @return ESP_OK si la escritura por I2C fue exitosa.
 */
esp_err_t rtc_write_time_in_module(struct tm *t);

#endif 