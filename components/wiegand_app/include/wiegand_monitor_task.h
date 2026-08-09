#ifndef WIEGAND_MONITOR_TASK_H
#define WIEGAND_MONITOR_TASK_H

/**
 * @file wiegand_monitor_task.h
 * @brief Tarea de escucha y ensamblaje de bits Wiegand.
 */

/**
 * @brief Extrae los bits crudos recogidos por la ISR y arma una tarjeta.
 *
 * Vigila el contexto `g_wiegand_ctx`. Si detecta que no han llegado bits en
 * 50ms (timeout), asume que la trama terminó. Luego deduce el formato (W26 o W34)
 * y encola la tarjeta leída para ser procesada.
 *
 * @par Ejemplo de uso:
 * @code
 * xTaskCreate(wiegand_monitor_task, "monitor", 4096, NULL, 12, NULL);
 * @endcode
 */
void wiegand_monitor_task(void *pvParameters);

#endif