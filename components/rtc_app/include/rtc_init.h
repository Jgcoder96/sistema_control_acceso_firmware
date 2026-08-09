#ifndef RTC_INIT_H
#define RTC_INIT_H

/**
 * @file rtc_init.h
 * @brief Inicialización de la tarea principal del RTC.
 */

#define RTC_MANAGEMENT_TASK_PRIO         5
#define RTC_MANAGEMENT_TASK_SIZE         4096 

/**
 * @brief Lanza la tarea asíncrona que gestiona el reloj.
 *
 * Configura I2C, ajusta el reloj del ESP32 con el módulo DS3231/DS1307
 * en el arranque, y se queda sincronizando la hora con NTP periódicamente.
 *
 * @par Ejemplo de uso:
 * @code
 * rtc_app_init();
 * @endcode
 */
void rtc_app_init(void);

#endif 