#include "esp_log.h"
#include "wifi_mesh_tasks.h"
#include "mqtt_publisher.h"

static const char *TAG = "NODE_LOGIC";

void handle_child_to_root(const app_packet_t *msg) {
  mqtt_publisher("mesh/config", *msg);
}

void handle_root_to_all_children(const app_packet_t *msg) {
  ESP_LOGW(TAG, "CONFIG GLOBAL | ACCION: %s", msg->payload.accion);
}

void handle_root_to_child(const app_packet_t *msg, const uint8_t *my_mac) {
  if (memcmp(msg->destination_mac, my_mac, 6) == 0) {
    ESP_LOGW(TAG, "CONFIG LOCAL | ACCION: %s", msg->payload.accion);
  }
}