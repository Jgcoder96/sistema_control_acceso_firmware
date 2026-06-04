#ifndef WIFI_MESH_INFO_H
#define WIFI_MESH_INFO_H

#include "esp_mesh.h"

typedef struct {
  mesh_addr_t mesh_id;   
  uint8_t mac[6];         
  int layer;              
  int rssi;              
  bool is_root; 
  bool is_internet_available;
  bool is_mesh_connected;
  bool is_mqtt_connected;
  bool is_synchronized;
  uint16_t next_page_to_request;

} node_mesh_info_t;

extern node_mesh_info_t node_mesh_info;

#endif