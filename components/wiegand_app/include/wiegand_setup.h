#ifndef WIEGAND_SETUP_H
#define WIEGAND_SETUP_H

#include "esp_err.h"
#include "wiegand_types.h"

/**
 * @file wiegand_setup.h
 * @brief Instalación de interrupciones Wiegand.
 */

/**
 * @brief Configura los pines D0 y D1 e instala sus ISR.
 *
 * A partir de esta llamada, cualquier pulso de bajada (flanco descendente)
 * en D0 o D1 desencadenará la ISR para capturar 0s y 1s.
 *
 * @par Ejemplo de uso:
 * @code
 * wiegand_setup(&WIEGAND_CONFIG_DEFAULT);
 * @endcode
 *
 * @param config Puntero a la configuración con los pines D0/D1.
 * @return ESP_OK si las interrupciones se instalaron correctamente.
 */
esp_err_t wiegand_setup(const wiegand_config_t *config);

#endif