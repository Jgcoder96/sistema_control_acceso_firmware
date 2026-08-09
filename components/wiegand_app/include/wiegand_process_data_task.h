#ifndef WIEGAND_PROCESS_DATA_TASK_H
#define WIEGAND_PROCESS_DATA_TASK_H

/**
 * @file wiegand_process_data_task.h
 * @brief Tarea de validación y reglas de negocio para accesos.
 */

/**
 * @brief Consume la cola del lector Wiegand, valida y acciona.
 *
 * Al desencolar un RFID:
 * 1. Comprueba si hoy es festivo o día normal.
 * 2. Verifica si la tarjeta existe en NVS y si la regla de hora coincide.
 * 3. Notifica a la tarea de hardware para abrir o denegar.
 * 4. Manda el registro por MQTT (Mesh) o a la NVS Offline.
 *
 * @par Ejemplo de uso:
 * @code
 * xTaskCreate(wiegand_process_data_task, "process", 4096, NULL, 10, NULL);
 * @endcode
 */
void wiegand_process_data_task(void *pvParameters);

#endif