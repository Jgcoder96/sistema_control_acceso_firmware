#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"

#include "wiegand_config.h"
#include "wiegand_init.h"
#include "wiegand_monitor_task.h"
#include "wiegand_process_data_task.h"
#include "wiegand_setup.h"

// static const char *TAG = "WIEGAND_INIT";

QueueHandle_t wiegand_reader_queue = NULL;

void wiegand_init(void) {
  wiegand_reader_queue = xQueueCreate(WIEGAND_READER_QUEUE_LENGTH, sizeof(wiegand_card_t));

  ESP_ERROR_CHECK(wiegand_setup(&WIEGAND_CONFIG_DEFAULT));

  xTaskCreatePinnedToCore(wiegand_monitor_task, "wiegand_monitor_task", WIEGAND_MONITOR_TASK_SIZE, NULL, WIEGAND_MONITOR_TASK_PRIO, NULL, 1);

  xTaskCreatePinnedToCore(wiegand_process_data_task, "wiegand_process_data_task", WIEGAND_PROCESS_DATA_TASK_SIZE, NULL, WIEGAND_PROCESS_DATA_TASK_PRIO, NULL, 1);
  
}