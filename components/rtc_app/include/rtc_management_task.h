#ifndef RTC_MANAGEMENT_TASK_H
#define RTC_MANAGEMENT_TASK_H

/**
 * @file rtc_management_task.h
 * @brief Tarea de gestión del tiempo (NTP y Mesh).
 */

/**
 * @brief Mantiene la hora actualizada y la difunde.
 *
 * Tarea en bucle que, si el nodo es Root y tiene internet, contacta a NTP 
 * periódicamente. Tras sincronizar, hace broadcast (Mesh) para que los Hijos ajusten su reloj.
 *
 * @par Ejemplo de uso:
 * @code
 * xTaskCreate(rtc_management_task, "rtc", 4096, NULL, 5, NULL);
 * @endcode
 */
void rtc_management_task(void *pvParameters);

#endif 