#ifndef MQTT_SUBSCRIPTION_MANAGER_H
#define MQTT_SUBSCRIPTION_MANAGER_H

#include "cJSON.h"

void subscription_manager_configure_device(cJSON *root);
void subscription_manager_configure_all_devices(cJSON *root);

#endif