#ifndef BUZZER_APP_H
#define BUZZER_APP_H

#include <stdint.h>

/**
 * @brief Inicializa el hardware (PWM) para el buzzer.
 */
void buzzer_app_init(void);

/**
 * @brief Hace sonar el buzzer a una frecuencia específica por un tiempo.
 * 
 * @param freq Frecuencia en Hz.
 * @param duration_ms Duración del pitido en milisegundos.
 */
void buzzer_app_beep(uint32_t freq, uint32_t duration_ms);

#endif // BUZZER_APP_H
