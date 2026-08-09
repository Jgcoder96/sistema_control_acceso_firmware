#ifndef HARDWARE_CONTROL_TASK_H
#define HARDWARE_CONTROL_TASK_H

/**
 * @file hardware_control_task.h
 * @brief Control físico (Relé y LEDs) tras evaluar una tarjeta.
 */

#define PIN_RELE      37
#define PIN_LED_L2    38
#define PIN_LED_L1    39

/**
 * @brief Estructura de comando enviada desde el validador hacia el hardware.
 */
typedef struct {
  bool permitted;
} hardware_cmd_t;

/**
 * @brief Tarea que escucha la cola de hardware y acciona los pines.
 *
 * Si recibe acceso permitido, enciende el relé y LED verde por unos 
 * segundos. Si denegado, enciende el LED rojo.
 *
 * @par Ejemplo de uso:
 * @code
 * xTaskCreate(hardware_control_task, "hw_ctrl", 4096, NULL, 10, NULL);
 * @endcode
 */
void hardware_control_task(void *pvParameters);

#endif