#ifndef RTC_I2C_CONFIG_H
#define RTC_I2C_CONFIG_H

#include "driver/i2c.h"

/**
 * @file rtc_i2c_config.h
 * @brief Configuración de bajo nivel del bus I2C.
 */

/**
 * @brief Inicializa el puerto I2C para hablar con el chip RTC.
 *
 * Utiliza los pines definidos en rtc_config.h (SDA, SCL) e instala el driver
 * del ESP32.
 *
 * @par Ejemplo de uso:
 * @code
 * if (rtc_i2c_config() == ESP_OK) {
 *    // I2C listo
 * }
 * @endcode
 */
esp_err_t rtc_i2c_config(void);

#endif