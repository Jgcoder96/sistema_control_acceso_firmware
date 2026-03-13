#include "cJSON.h"
#include <time.h>

#include "app_types.h"

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
  }

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