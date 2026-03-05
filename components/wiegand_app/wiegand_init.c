#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"

#include "wiegand_config.h"
#include "wiegand_monitor_task.h"
#include "wiegand_process_data_task.h"
#include "wiegand_setup.h"

QueueHandle_t wiegand_reader_queue = NULL;

void wiegand_init(void) {
  wiegand_reader_queue = xQueueCreate(WIEGAND_QUEUE_LENGTH, sizeof(wiegand_card_t));

  ESP_ERROR_CHECK(wiegand_setup(&WIEGAND_CONFIG_DEFAULT));

  xTaskCreatePinnedToCore(wiegand_monitor_task,"wiegand_monitor_task",4096, NULL, WIEGAND_DRIVER_PRIO, NULL,1);

  xTaskCreatePinnedToCore(wiegand_process_data_task, "wiegand_process_data_task", 4096, NULL, CARD_PROCESSOR_PRIO, NULL, 1);
  
}