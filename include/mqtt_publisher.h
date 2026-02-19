#ifndef MQTT_PUBLISHER_H
#define MQTT_PUBLISHER_H

#include "wifi_mesh_tasks.h"

void mqtt_publisher(const char *topic, app_packet_t packet);

#endif