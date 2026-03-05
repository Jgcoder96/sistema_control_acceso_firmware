#ifndef MQTT_PUBLISHER_H
#define MQTT_PUBLISHER_H

#include "app_types.h"

void mqtt_publisher(const char *topic, app_packet_t packet);

#endif