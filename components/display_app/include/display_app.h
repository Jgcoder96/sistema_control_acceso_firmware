#ifndef DISPLAY_APP_H
#define DISPLAY_APP_H

/**
 * @file display_app.h
 * @brief Interfaz de aplicación para el control de la pantalla OLED.
 *
 * Proporciona funciones a otros componentes para mostrar notificaciones 
 * visuales de forma asíncrona.
 */

#include <stdbool.h>

/**
 * @brief Muestra un popup temporal en la pantalla indicando el estado de acceso.
 *
 * Modifica las variables de estado leídas por la tarea de la pantalla OLED para
 * renderizar un mensaje de "ACCESO CONCEDIDO" o "ACCESO DENEGADO" durante 2.5 segundos.
 *
 * @param granted True si el acceso fue concedido, False si fue denegado.
 */
void display_app_show_popup(bool granted);

#endif // DISPLAY_APP_H
