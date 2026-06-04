#include <sys/time.h>  
#include <inttypes.h>   
#include "esp_log.h"

#include "app_types.h"
#include "mqtt_publisher.h"
#include "rtc_write_time_in_module.h"
#include "wifi_mesh_info.h"
#include "sync_rtc.h"
#include "sync_mqtt_status.h"
#include "mqtt_manager.h"
#include "show_permissions_to_sync.h"
#include "nvs_save_permissions.h"
#include "sync_device_trigger.h"


//static const char *TAG = "WIFI_MESH_NODE_LOGIC";

void handle_child_to_root(const app_packet_t *msg) {
  switch (msg->msg_type) {
    case MSG_TYPE_CARD:
      mqtt_publisher("mesh/config", *msg);
      break;

    case MSG_TYPE_INITIAL_SYNC:
      mqtt_publisher("device/sync/request", *msg);
      break;

    case MSG_TYPE_SYNC_ACK:
      mqtt_publisher("device/sync/successful", *msg);
      break;

    default:     
      break;
  }
}

void handle_root_to_all_children(const app_packet_t *msg) {
  switch (msg->msg_type) {
    case MSG_TYPE_RTC_SYNC:
      if (!node_mesh_info.is_root) {
        sync_rtc(msg->payload.timestamp);
      }
      break;

    case MSG_TYPE_MQTT_STATUS:
      if (!node_mesh_info.is_root) {
        sync_mqtt_status(msg->payload.mqtt_status_event.mqtt_status);
      }
      break;

    default:      
      break;
  }
}

void handle_root_to_child(const app_packet_t *msg, const uint8_t *my_mac) {
  if (memcmp(msg->destination_mac, node_mesh_info.mac, 6) != 0) {
    return;
  }

  switch (msg->msg_type) {  

    case MSG_TYPE_SYNC_DATA:    
      show_permissions_to_sync(&msg->payload.sync_data_event);
      nvs_save_permissions(&msg->payload.sync_data_event);
      break;
    
    case MSG_TYPE_SYNC_TRIGGER:
      if (msg->payload.sync_trigger_event.execute) {
        sync_device_trigger();
      }
      break;

    default:
      break;
  }
}