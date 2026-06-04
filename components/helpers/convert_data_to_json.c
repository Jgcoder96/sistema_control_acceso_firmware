#include "cJSON.h"
#include <time.h>

#include "app_types.h"
#include "wifi_mesh_info.h"

char* packet_to_json(app_packet_t packet) {
  cJSON *root = cJSON_CreateObject();
  
  if (root == NULL) return NULL;

  if (packet.msg_type == MSG_TYPE_CARD) {
    cJSON_AddStringToObject(root, "card_id", packet.payload.access_event.card_id);

    time_t t = (time_t)packet.payload.access_event.timestamp;
    
    struct tm timeinfo;

    localtime_r(&t, &timeinfo);
      
    char formatted_date[32];
    strftime(formatted_date, sizeof(formatted_date), "%d/%m/%Y %H:%M:%S", &timeinfo);
    cJSON_AddStringToObject(root, "date", formatted_date);
  } else if (packet.msg_type == MSG_TYPE_INITIAL_SYNC) {
    cJSON_AddStringToObject(root, "type", "sync_request");
    
    cJSON_AddNumberToObject(root, "version", packet.payload.initial_sync_event.version);
    
    if (packet.payload.initial_sync_event.state == SYNC_STATE_NONE) {
      cJSON_AddBoolToObject(root, "full_sync_required", true);
    } else {
      cJSON_AddBoolToObject(root, "full_sync_required", false);
    }
  } else if (packet.msg_type == MSG_TYPE_SYNC_ACK) {
    cJSON_AddStringToObject(root, "type", "sync_successful");
    cJSON_AddNumberToObject(root, "version", packet.payload.sync_ack_event.version);
    cJSON_AddBoolToObject(root, "success", packet.payload.sync_ack_event.success);
  }

  char mac_src[18];

  snprintf(mac_src, sizeof(mac_src), "%02X:%02X:%02X:%02X:%02X:%02X", 
           packet.source_mac[0], packet.source_mac[1], 
           packet.source_mac[2], packet.source_mac[3], 
           packet.source_mac[4], packet.source_mac[5]);

  cJSON_AddStringToObject(root, "device_mac", mac_src);

  char mesh_id_str[18];

  snprintf(mesh_id_str, sizeof(mesh_id_str), "%02X:%02X:%02X:%02X:%02X:%02X", 
           node_mesh_info.mesh_id.addr[0], node_mesh_info.mesh_id.addr[1], 
           node_mesh_info.mesh_id.addr[2], node_mesh_info.mesh_id.addr[3], 
           node_mesh_info.mesh_id.addr[4], node_mesh_info.mesh_id.addr[5]);
           
  cJSON_AddStringToObject(root, "mesh_id", mesh_id_str);

  char *json_str = cJSON_PrintUnformatted(root);
    
  cJSON_Delete(root);
    
  return json_str;
}