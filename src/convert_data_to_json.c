#include "cJSON.h"

#include "wifi_mesh_tasks.h"

char* packet_to_json(app_packet_t packet) {
  cJSON *root = cJSON_CreateObject();
  if (root == NULL) return NULL;

  cJSON_AddNumberToObject(root, "layer", packet.layer);
  cJSON_AddNumberToObject(root, "card_id", packet.card_id);

  char mac_src[18];
  snprintf(mac_src, sizeof(mac_src), "%02X:%02X:%02X:%02X:%02X:%02X", 
           packet.source_mac[0], packet.source_mac[1], 
           packet.source_mac[2], packet.source_mac[3], 
           packet.source_mac[4], packet.source_mac[5]);

  cJSON_AddStringToObject(root, "device_mac", mac_src);

  char *json_str = cJSON_PrintUnformatted(root);
    
  cJSON_Delete(root);
    
  return json_str;
}