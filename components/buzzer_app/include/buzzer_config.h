#ifndef BUZZER_CONFIG_H
#define BUZZER_CONFIG_H

/**
 * @file buzzer_config.h
 * @brief Configuraciones generales de hardware para el módulo Buzzer.
 *
 * Contiene las definiciones de pines, temporizadores y canales del periférico
 * LEDC (LED Control) de Espressif, utilizado para generar la señal PWM del buzzer.
 */

#include "driver/ledc.h"

/// Pin GPIO donde está conectado el buzzer
#define BUZZER_PIN          (40)

/// Temporizador de hardware utilizado para el PWM
#define LEDC_TIMER          LEDC_TIMER_0

/// Modo de velocidad del periférico LEDC
#define LEDC_MODE           LEDC_LOW_SPEED_MODE

/// Canal LEDC asignado para la señal PWM
#define LEDC_CH0_CHANNEL    LEDC_CHANNEL_0

/// Resolución del ciclo de trabajo (10 bits = valores de 0 a 1023)
#define LEDC_DUTY_RES       LEDC_TIMER_10_BIT

/// Ciclo de trabajo por defecto (512 = 50% de 1023) para onda cuadrada perfecta
#define LEDC_DUTY           (512)

#endif // BUZZER_CONFIG_H
