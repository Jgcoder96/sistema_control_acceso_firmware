#ifndef DISPLAY_APP_H
#define DISPLAY_APP_H

#include <stdbool.h>

/**
 * @brief Inicializa la pantalla OLED I2C e inicia la tarea de actualización.
 */
void display_app_init(void);
void display_app_show_popup(bool granted);

#endif // DISPLAY_APP_H
