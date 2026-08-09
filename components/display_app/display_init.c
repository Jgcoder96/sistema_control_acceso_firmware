/**
 * @file display_init.c
 * @brief Implementación de la inicialización del módulo de Pantalla OLED.
 *
 * Se encarga de instanciar y despachar la tarea de FreeRTOS que controlará el display.
 */

#include "display_init.h"
#include "display_task.h"
#include "esp_log.h"

static const char *TAG = "DISPLAY_INIT";

void display_app_init(void) {
    xTaskCreate(display_task, "display_task", DISPLAY_TASK_SIZE, NULL, DISPLAY_TASK_PRIO, NULL);
    ESP_LOGI(TAG, "Tarea de pantalla creada (tamaño: %d, prioridad: %d)", DISPLAY_TASK_SIZE, DISPLAY_TASK_PRIO);
}
