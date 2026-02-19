#include "esp_log.h"
#include "esp_mac.h" 
#include <string.h>

#include "mqtt_manager.h"
#include "wifi_mesh_transmission.h"
#include "mqtt_publisher.h"

//static const char *TAG = "WIFI_MESH_TASKS";

bool is_mesh_connected = false;
int my_mesh_layer = -1;
uint8_t my_mac[6];

void mesh_transmitter_task(void *arg) {
  app_packet_t app_packet;
  while (1) {
    if (xQueueReceive(mesh_tx_queue, &app_packet, portMAX_DELAY) == pdTRUE) {
      mesh_data_t data = {
        .data = (uint8_t *)&app_packet, 
        .size = sizeof(app_packet), 
        .proto = MESH_PROTO_BIN, 
        .tos = MESH_TOS_P2P
      };
      mesh_addr_t target;
      mesh_addr_t *dest_ptr = NULL;

      if (esp_mesh_is_root()) {
        memcpy(target.addr, app_packet.destination_mac, 6);
        dest_ptr = &target;
      }

      esp_mesh_send(dest_ptr, &data, MESH_DATA_P2P, NULL, 0);
    }
  }
}

void mesh_receiver_task(void *arg) {
  mesh_addr_t source_mac;
  int flag = 0;
  static uint8_t rx_buf[512];
  while (1) {
    mesh_data_t data = {.data = rx_buf, .size = sizeof(rx_buf)};

    if (esp_mesh_recv(&source_mac, &data, portMAX_DELAY, &flag, NULL, 0) == ESP_OK) {
      mesh_packet_t q_msg;
      q_msg.len = (data.size > sizeof(q_msg.data)) ? sizeof(q_msg.data) : data.size;
      q_msg.source_mac = source_mac;
      memcpy(q_msg.data, data.data, q_msg.len);

      xQueueSend(mesh_rx_queue, &q_msg, 0);
    }
  }
}

void mesh_processor_task(void *arg) {
  mesh_packet_t q_msg;
  while (1) {
    if (xQueueReceive(mesh_rx_queue, &q_msg, portMAX_DELAY) == pdTRUE) {
      if (q_msg.len != sizeof(app_packet_t)) continue;

      app_packet_t *msg = (app_packet_t *)q_msg.data;

      switch (msg->type) {
        case CHILD_TO_ROOT:
          mqtt_publisher("mesh/config", *msg);
          break;
        case ROOT_TO_ALL_CHILDREN:
          break;
        case ROOT_TO_CHILD:
          if (memcmp(msg->destination_mac, my_mac, 6) == 0) {
          }
          break;
      }
    }
  }
}