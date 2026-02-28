#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "mqtt_client.h"

#include "mqtt_config.h"
#include "mqtt_publisher.h"
#include "wiegand_types.h"
#include "wifi_mesh_tasks.h"
#include "wifi_mesh_transmission.h"

extern QueueHandle_t wiegand_reader_queue;

extern bool is_mesh_connected;
extern int my_mesh_layer;
extern uint8_t my_mac[6];
extern bool is_node_root;

void process_data_from_wiegand_reader_task(void *pvParameters) {
  wiegand_card_t card;

  while (1) {    
    if (xQueueReceive(wiegand_reader_queue, &card, portMAX_DELAY) == pdTRUE) {

      app_packet_t data = {0};
      memcpy(data.source_mac, my_mac, 6);
      data.layer = my_mesh_layer;
      data.payload.card_id = card.full_id;

      send_upstream(&data);
    }
  }
}