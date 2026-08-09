#ifndef RTC_SET_TIME_FROM_NTP_SERVER_H
#define RTC_SET_TIME_FROM_NTP_SERVER_H

#include <stdbool.h>

/**
 * @file rtc_set_time_from_ntp_server.h
 * @brief Sincronización de hora mediante protocolo NTP.
 */

/**
 * @brief Conecta a los servidores NTP definidos y extrae la hora.
 *
 * Si es exitoso, actualiza el reloj interno y guarda la hora en el 
 * módulo externo I2C (DS3231/DS1307).
 *
 * @par Ejemplo de uso:
 * @code
 * if (set_time_from_npt_server()) {
 *    printf("Reloj sincronizado con internet\n");
 * }
 * @endcode
 *
 * @return true si pudo conectar y ajustar el reloj, false en caso contrario.
 */
bool set_time_from_npt_server(void);

#endif 