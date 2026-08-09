/**
 * @file hardware_control_task.c
 * @brief Implementación del accionamiento físico (Relé/LEDs).
 */
#include <stdbool.h>       
#include "freertos/FreeRTOS.h" 
#include "freertos/task.h"   
#include "freertos/queue.h"    
#include "driver/gpio.h"   
#include "hardware_control_task.h"

extern QueueHandle_t hardware_control_queue;

void hardware_control_task(void *pvParameters) {
  gpio_config_t io_conf = {
    .intr_type = GPIO_INTR_DISABLE,
    .mode = GPIO_MODE_OUTPUT,
    .pin_bit_mask = (1ULL << PIN_RELE) | (1ULL << PIN_LED_L2) | (1ULL << PIN_LED_L1),
  };

  gpio_config(&io_conf);

  hardware_cmd_t action;

  while (1) {
    // Esperar comandos desde la tarea de procesamiento
    if (xQueueReceive(hardware_control_queue, &action, portMAX_DELAY) == pdTRUE) {
      if (action.permitted) {
        // Acceso Permitido: Relé ON, LED Verde ON, LED Rojo OFF
        gpio_set_level(PIN_RELE, 1);
        gpio_set_level(PIN_LED_L2, 1);
        gpio_set_level(PIN_LED_L1, 0);
      } else {
        // Acceso Denegado: Relé OFF, LED Verde OFF, LED Rojo ON
        gpio_set_level(PIN_RELE, 0);
        gpio_set_level(PIN_LED_L2, 0);
        gpio_set_level(PIN_LED_L1, 1);
      }

      // Mantener el estado por 3 segundos
      vTaskDelay(pdMS_TO_TICKS(3000));

      // Volver a estado de reposo (Todo apagado)
      gpio_set_level(PIN_RELE, 0);
      gpio_set_level(PIN_LED_L2, 0);
      gpio_set_level(PIN_LED_L1, 0);
    }
  }
}