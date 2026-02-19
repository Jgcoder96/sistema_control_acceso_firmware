#ifndef WIFI_MESH_EVENT_HANDLER_H
#define WIFI_MESH_EVENT_HANDLER_H

#include "esp_event.h"
#include "esp_netif.h"

extern bool is_internet_available;
extern bool is_node_root;
extern esp_netif_t *netif_sta;

void wifi_mesh_event_handler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data);

#endif