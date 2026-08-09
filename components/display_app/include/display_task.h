#ifndef DISPLAY_TASK_H
#define DISPLAY_TASK_H

/**
 * @file display_task.h
 * @brief Tarea asíncrona de la Pantalla OLED y variables compartidas.
 *
 * Contiene la definición de la tarea principal de actualización y las
 * variables utilizadas para mostrar notificaciones o popups en la pantalla.
 */

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

/// Estado del popup actual: 0 = Normal, 1 = Acceso Concedido, 2 = Acceso Denegado
extern int current_popup;

/// Tick de sistema (FreeRTOS) en el cual el popup debe desaparecer
extern uint32_t popup_timeout_tick;

/**
 * @brief Bucle principal de actualización de la pantalla OLED.
 *
 * Se ejecuta infinitamente refrescando la información mostrada cada 1 segundo.
 * Realiza la inicialización de los pines I2C por software (bit-banging).
 *
 * @param pvParameters Parámetros de la tarea (no utilizado).
 */
void display_task(void *pvParameters);

#endif // DISPLAY_TASK_H
