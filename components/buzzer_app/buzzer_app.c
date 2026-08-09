/**
 * @file buzzer_app.c
 * @brief Implementación de la interfaz de la aplicación del Buzzer.
 *
 * Facilita el uso del buzzer encapsulando el empaquetado del comando
 * y su envío a la cola asíncrona de FreeRTOS.
 */

#include "buzzer_app.h"
#include "buzzer_task.h"
#include "esp_log.h"

void buzzer_app_beep(uint32_t freq, uint32_t duration_ms) {
    if (buzzer_queue != NULL) {
        buzzer_cmd_t cmd = {
            .freq = freq,
            .duration_ms = duration_ms
        };
        // Enviamos el comando a la cola con un timeout de 0, para no bloquear
        xQueueSend(buzzer_queue, &cmd, 0);
    }
}
