#ifndef BUZZER_INIT_H
#define BUZZER_INIT_H

/**
 * @file buzzer_init.h
 * @brief Interfaz de inicialización del módulo Buzzer.
 *
 * Contiene la declaración de la función responsable de configurar el hardware 
 * (timers y canales PWM) e instanciar la tarea asíncrona de FreeRTOS.
 */

/**
 * @brief Inicializa el hardware (PWM) para el buzzer y crea su tarea de FreeRTOS.
 *
 * Esta función debe llamarse una sola vez al inicio del sistema (`app_main`).
 * Configura los registros LEDC del ESP32 para generar ondas cuadradas,
 * reserva la cola de mensajes y crea la tarea de procesamiento de pitidos.
 */
void buzzer_app_init(void);

#endif // BUZZER_INIT_H
