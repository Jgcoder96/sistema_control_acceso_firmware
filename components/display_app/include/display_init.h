#ifndef DISPLAY_INIT_H
#define DISPLAY_INIT_H

/**
 * @file display_init.h
 * @brief Interfaz de inicialización de la pantalla OLED.
 *
 * Expone la función para lanzar la tarea principal de actualización y renderizado
 * de la pantalla.
 */

/// Tamaño de la pila (stack) reservada para la tarea de la pantalla OLED
#define DISPLAY_TASK_SIZE 8192

/// Prioridad de la tarea de la pantalla OLED en el scheduler
#define DISPLAY_TASK_PRIO 5

/**
 * @brief Crea e inicia la tarea de FreeRTOS encargada de controlar la pantalla OLED.
 *
 * Esta función debe ser llamada una sola vez durante la secuencia de arranque 
 * en el `app_main`.
 */
void display_app_init(void);

#endif // DISPLAY_INIT_H
