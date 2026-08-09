#ifndef WIEGAND_TYPES_H
#define WIEGAND_TYPES_H

#include "driver/gpio.h"

/**
 * @file wiegand_types.h
 * @brief Definiciones de tipos para tarjetas y configuraciones RFID.
 */

/**
 * @brief Formatos soportados por el lector.
 */
typedef enum {
  WIEGAND_FORMAT_UNKNOWN = 0,
  WIEGAND_FORMAT_W26     = 26,
  WIEGAND_FORMAT_W34     = 34
} wiegand_format_t;

/**
 * @brief Datos extraídos y parseados de un intento de lectura RFID.
 */
typedef struct {
  uint64_t          raw_bits;          ///< Cadena completa de bits crudos
  uint64_t          full_id;           ///< ID completo sin paridad
  uint32_t          facility_code;     ///< Código de facilidad (W26)
  uint32_t          card_id;           ///< Código de tarjeta (W26)
  wiegand_format_t  format;            ///< Formato detectado (W26, W34, etc)
  int               bit_count;         ///< Total de bits recibidos
} wiegand_card_t;

/**
 * @brief Estructura de pines a usar.
 */
typedef struct {
  gpio_num_t        gpio_d0;
  gpio_num_t        gpio_d1;
} wiegand_config_t;

#endif