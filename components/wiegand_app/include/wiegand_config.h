#ifndef WIEGAND_CONFIG_H
#define WIEGAND_CONFIG_H

#include "wiegand_types.h"

/**
 * @file wiegand_config.h
 * @brief Configuración estática y pines para el bus Wiegand.
 */

/// Pin asignado para la línea D0 de Wiegand
#define WIEGAND_GPIO_D0 GPIO_NUM_11
/// Pin asignado para la línea D1 de Wiegand
#define WIEGAND_GPIO_D1 GPIO_NUM_12

/// Estructura de configuración por defecto para inyectar al inicializar
static const wiegand_config_t WIEGAND_CONFIG_DEFAULT = {
  .gpio_d0 = WIEGAND_GPIO_D0,
  .gpio_d1 = WIEGAND_GPIO_D1
};

#endif