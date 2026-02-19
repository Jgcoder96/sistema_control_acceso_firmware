#include "wifi_mesh_tasks.h"

//static const char *TAG = "WIFI_MESH_TRANSMISSION";

static void send_mesh_packet(app_packet_t *packet, type_t type, const uint8_t *destination_mac, const char *payload) {
  app_packet_t app_packet = *packet;
  app_packet.type = type;

  if (destination_mac) memcpy(app_packet.destination_mac, destination_mac, 6);
  else memset(app_packet.destination_mac, 0, 6);

  xQueueSend(mesh_tx_queue, &app_packet, portMAX_DELAY);
}

void send_upstream(app_packet_t *data) {
  char buf[48];
  snprintf(buf, sizeof(buf), "[HIJO (capa: %d) -> ROOT]", data->layer);
  send_mesh_packet(data, CHILD_TO_ROOT, NULL, buf);
}

void broadcast_to_mesh(app_packet_t *data) {
  uint8_t broadcast_mac[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
  char buf[48];
  snprintf(buf, sizeof(buf), "[ROOT (capa: %d) -> BROADCAST]", data->layer);
  send_mesh_packet(data, ROOT_TO_ALL_CHILDREN, broadcast_mac, buf);
}

void send_downstream(app_packet_t *data) {
  mesh_addr_t route_table[20];
  int route_size = 0;
  char buf[48];

  if (esp_mesh_get_routing_table(route_table, sizeof(route_table), &route_size) == ESP_OK && route_size > 1) {
    snprintf(buf, sizeof(buf), "[ROOT (capa: %d) -> HIJO]", data->layer);
    send_mesh_packet(data, ROOT_TO_CHILD, route_table[1].addr, buf);
  }
}