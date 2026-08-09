#ifndef BUZZER_APP_H
#define BUZZER_APP_H

/**
 * @file buzzer_app.h
 * @brief Interfaz de aplicación para el control del buzzer.
 *
 * Proporciona a otros componentes (como el lector Wiegand) una función sencilla
 * y asíncrona para emitir pitidos de alerta o confirmación.
 */

#include <stdint.h>

// La inicialización ahora se encuentra en buzzer_init.h

/**
 * @brief Emite un sonido a través del buzzer de forma asíncrona.
 *
 * Envía un comando a la cola de la tarea del buzzer y retorna de forma
 * instantánea, sin bloquear el hilo de ejecución que la llama.
 * 
 * @param freq Frecuencia del sonido en Hertzios (Hz). Ejemplo: 2000.
 * @param duration_ms Duración total del pitido en milisegundos.
 */
void buzzer_app_beep(uint32_t freq, uint32_t duration_ms);

#endif // BUZZER_APP_H
