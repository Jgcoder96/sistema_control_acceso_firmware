#ifndef MQTT_SUBSCRIPTION_MANAGER_H
#define MQTT_SUBSCRIPTION_MANAGER_H

#include "cJSON.h"

void procesar_mensaje_sincronizacion(cJSON *root);
void subscription_manager_sync_trigger(cJSON *root);

#endif