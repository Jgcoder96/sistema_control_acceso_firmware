#include "wifi_mesh_tasks.h"

void handle_child_to_root(const app_packet_t *msg);
void handle_root_to_all_children(const app_packet_t *msg);
void handle_root_to_child(const app_packet_t *msg, const uint8_t *my_mac);