#ifndef WIEGAND_INIT_H
#define WIEGAND_INIT_H

/**
 * @file wiegand_init.h
 * @brief Inicialización de colas y tareas del módulo Wiegand.
 */

#define WIEGAND_READER_QUEUE_LENGTH       10
#define HARDWARE_CONTROL_QUEUE_LENGTH     10

#define WIEGAND_MONITOR_TASK_PRIO         12
#define WIEGAND_MONITOR_TASK_SIZE         4096 

#define WIEGAND_PROCESS_DATA_TASK_PRIO    10 
#define WIEGAND_PROCESS_DATA_TASK_SIZE    4096

#define HARDWARE_CONTROL_TASK_TASK_PRIO    10 
#define HARDWARE_CONTROL_TASK_TASK_SIZE    4096

/**
 * @brief Arranca todo el entorno de lectura RFID Wiegand.
 *
 * Crea las colas de comunicación entre tareas, inicializa el hardware y 
 * lanza los tres hilos principales: monitor (escucha), procesamiento y hardware.
 *
 * @par Ejemplo de uso:
 * @code
 * void app_main() {
 *     wiegand_init();
 * }
 * @endcode
 */
void wiegand_init(void);

#endif