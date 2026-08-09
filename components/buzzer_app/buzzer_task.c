/**
 * @file buzzer_task.c
 * @brief Implementación de la tarea asíncrona del Buzzer.
 *
 * Contiene el bucle principal de FreeRTOS que espera comandos en la cola
 * para activar la señal PWM en el hardware y producir sonidos no bloqueantes.
 */

#include "buzzer_task.h"
#include "buzzer_config.h"

QueueHandle_t buzzer_queue = NULL;

void buzzer_task(void *pvParameters) {
    buzzer_cmd_t cmd;

    while (1) {
        if (xQueueReceive(buzzer_queue, &cmd, portMAX_DELAY) == pdTRUE) {
            if (cmd.freq > 0) {
                // Configurar la nueva frecuencia
                ledc_set_freq(LEDC_MODE, LEDC_TIMER, cmd.freq);
                // Establecer el duty cycle al 50% para generar la onda
                ledc_set_duty(LEDC_MODE, LEDC_CH0_CHANNEL, LEDC_DUTY);
                ledc_update_duty(LEDC_MODE, LEDC_CH0_CHANNEL);
            } else {
                // Si la frecuencia es 0, silenciar
                ledc_set_duty(LEDC_MODE, LEDC_CH0_CHANNEL, 0);
                ledc_update_duty(LEDC_MODE, LEDC_CH0_CHANNEL);
            }

            // Esperar el tiempo indicado
            if (cmd.duration_ms > 0) {
                vTaskDelay(pdMS_TO_TICKS(cmd.duration_ms));
            }

            // Apagar después de que pase el tiempo
            ledc_set_duty(LEDC_MODE, LEDC_CH0_CHANNEL, 0);
            ledc_update_duty(LEDC_MODE, LEDC_CH0_CHANNEL);
        }
    }
}
