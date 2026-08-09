/**
 * @file wiegand_init.c
 * @brief Implementación de la inicialización y creación de tareas Wiegand.
 */
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "esp_log.h"

#include "hardware_control_task.h"
#include "wiegand_config.h"
#include "wiegand_init.h"
#include "wiegand_monitor_task.h"
#include "wiegand_process_data_task.h"
#include "wiegand_setup.h"

static const char *TAG = "WIEGAND_INIT";

QueueHandle_t wiegand_reader_queue = NULL;
QueueHandle_t hardware_control_queue = NULL;

void wiegand_init(void) {
  // Cola donde el monitor (escucha) le pasará las tarjetas a la tarea de procesamiento
  wiegand_reader_queue = xQueueCreate(WIEGAND_READER_QUEUE_LENGTH, sizeof(wiegand_card_t));
  // Cola donde la tarea de procesamiento le pide a la de hardware que abra la puerta
  hardware_control_queue = xQueueCreate(HARDWARE_CONTROL_QUEUE_LENGTH, sizeof(hardware_cmd_t));

  // Instalar Interrupciones
  ESP_ERROR_CHECK(wiegand_setup(&WIEGAND_CONFIG_DEFAULT));

  // Hilo 1: Ensambla bits y detecta cuándo termina de leer la tarjeta
  xTaskCreatePinnedToCore(wiegand_monitor_task, "wiegand_monitor_task", WIEGAND_MONITOR_TASK_SIZE, NULL, WIEGAND_MONITOR_TASK_PRIO, NULL, 1);

  // Hilo 2: Revisa permisos en la BD, suena el buzzer y manda eventos por Mesh/NVS
  xTaskCreatePinnedToCore(wiegand_process_data_task, "wiegand_process_data_task", WIEGAND_PROCESS_DATA_TASK_SIZE, NULL, WIEGAND_PROCESS_DATA_TASK_PRIO, NULL, 1);

  // Hilo 3: Activa Relé y LEDs según el resultado del procesamiento
  xTaskCreatePinnedToCore(hardware_control_task, "hardware_control_task", HARDWARE_CONTROL_TASK_TASK_SIZE, NULL, HARDWARE_CONTROL_TASK_TASK_PRIO, NULL, 1);

  ESP_LOGW(TAG, "[WIEGAND] Lector Wiegand iniciado correctamente.");
  
}