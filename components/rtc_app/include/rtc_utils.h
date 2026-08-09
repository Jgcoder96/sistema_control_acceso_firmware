#ifndef RTC_UTILS_H
#define RTC_UTILS_H

#include <stdint.h>

/**
 * @file rtc_utils.h
 * @brief Utilidades para conversión BCD-Decimal.
 */

/**
 * @brief Convierte de formato BCD a Decimal.
 *
 * Utilizado al leer los registros I2C del chip RTC, ya que el hardware 
 * entrega la hora en formato Binario Codificado en Decimal (BCD).
 */
int bcd_to_decimal(uint8_t val);

/**
 * @brief Convierte de Decimal a formato BCD.
 *
 * Utilizado al escribir la hora obtenida por NTP hacia el chip RTC.
 */
uint8_t decimal_to_bcd(int val);

#endif 