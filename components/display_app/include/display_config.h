#ifndef DISPLAY_CONFIG_H
#define DISPLAY_CONFIG_H

/**
 * @file display_config.h
 * @brief Configuración de hardware para la pantalla OLED.
 *
 * Contiene los pines y la dirección I2C de la pantalla OLED.
 */

/// Pin GPIO para la línea de datos I2C (SDA)
#define SDA_PIN 8

/// Pin GPIO para la línea de reloj I2C (SCL)
#define SCL_PIN 9

/// Dirección I2C de la pantalla OLED (típicamente 0x3C para 0.96")
#define OLED_ADDR 0x3C

#endif // DISPLAY_CONFIG_H
