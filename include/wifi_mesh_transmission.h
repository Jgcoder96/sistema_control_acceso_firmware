#ifndef WIFI_MESH_TRANSMISSION_H
#define WIFI_MESH_TRANSMISSION_H

#include "wifi_mesh_tasks.h"

void broadcast_to_mesh(app_packet_t *data);
void send_downstream(app_packet_t *data);
void send_upstream(app_packet_t *data);

#endif