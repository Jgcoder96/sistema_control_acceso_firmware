/**
 * @file display_app.c
 * @brief Implementación de la interfaz de la aplicación de la Pantalla OLED.
 *
 * Contiene funciones para interactuar con la tarea asíncrona de la pantalla,
 * modificando su estado interno (popups).
 */

#include "display_app.h"
#include "display_task.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

void display_app_show_popup(bool granted) {
    current_popup = granted ? 1 : 2;
    // Mostrar por 2.5 segundos (2500 ms)
    popup_timeout_tick = xTaskGetTickCount() + pdMS_TO_TICKS(2500);
}
