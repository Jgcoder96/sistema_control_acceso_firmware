#ifndef RTC_SYNC_DEVICE_INTERNAL_CLOCK_H
#define RTC_SYNC_DEVICE_INTERNAL_CLOCK_H

/**
 * @file rtc_sync_device_internal_clock.h
 * @brief Respaldo de hora desde hardware externo.
 */

/**
 * @brief Carga la hora del módulo I2C hacia el sistema operativo.
 *
 * Útil en el arranque o cuando no hay internet. Lee los registros del DS3231,
 * los convierte a un epoch y ajusta el `settimeofday` del ESP32.
 *
 * @par Ejemplo de uso:
 * @code
 * rtc_i2c_config();
 * rtc_sync_device_internal_clock();
 * @endcode
 */
void rtc_sync_device_internal_clock(void);

#endif 