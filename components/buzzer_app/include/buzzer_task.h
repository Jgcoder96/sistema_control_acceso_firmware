#ifndef BUZZER_TASK_H
#define BUZZER_TASK_H

/**
 * @file buzzer_task.h
 * @brief Tarea asíncrona de FreeRTOS para el manejo de sonidos.
 *
 * Expone las estructuras y recursos compartidos (cola de mensajes) utilizados
 * para emitir comandos sonoros sin bloquear los hilos principales de ejecución.
 */

#include <stdint.h>
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "freertos/task.h"

/// Tamaño en bytes reservado para la pila (stack) de la tarea del buzzer
#define BUZZER_TASK_SIZE 2048

/// Prioridad asignada a la tarea en el scheduler de FreeRTOS
#define BUZZER_TASK_PRIO 4

/**
 * @struct buzzer_cmd_t
 * @brief Estructura del comando enviado a la cola del buzzer.
 */
typedef struct {
    uint32_t freq;         ///< Frecuencia en Hertzios (Hz). 0 para silenciar.
    uint32_t duration_ms;  ///< Duración del sonido en milisegundos.
} buzzer_cmd_t;

/// Identificador de la cola de mensajes global del buzzer
extern QueueHandle_t buzzer_queue;

/**
 * @brief Tarea de FreeRTOS que procesa los pitidos.
 *
 * Bloquea pasivamente (`xQueueReceive`) hasta que llega un nuevo comando.
 * Controla directamente los registros del hardware LEDC.
 *
 * @param pvParameters Parámetros pasados al instanciar la tarea (no usado).
 */
void buzzer_task(void *pvParameters);

#endif // BUZZER_TASK_H
