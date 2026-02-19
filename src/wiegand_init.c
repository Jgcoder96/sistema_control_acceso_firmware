#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"

#include "process_data_from_wiegand_reader_task.h"
#include "wiegand_config.h"
#include "wiegand_reader.h"

QueueHandle_t wiegand_reader_queue = NULL;

void wiegand_reader_init(void) {
  wiegand_reader_queue = xQueueCreate(WIEGAND_QUEUE_LENGTH, sizeof(wiegand_card_t));

  ESP_ERROR_CHECK(wiegand_init(&WIEGAND_CONFIG_DEFAULT));

  xTaskCreatePinnedToCore(wiegand_monitor_task,"wiegand_monitor",4096, NULL, WIEGAND_DRIVER_PRIO, NULL,1);

  xTaskCreatePinnedToCore(process_data_from_wiegand_reader_task, "process_data_from_wiegand_reader_task", 4096, NULL, CARD_PROCESSOR_PRIO, NULL, 1);
  
}