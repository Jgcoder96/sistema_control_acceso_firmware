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


static const char *TAG = "WIFI_MESH_NODE_LOGIC";

void handle_child_to_root(const app_packet_t *msg) {
  if (msg->msg_type == MSG_TYPE_CARD) {
    mqtt_publisher("mesh/config", *msg);
  } else if (msg->msg_type == MSG_TYPE_INITIAL_SYNC) {
    mqtt_publisher("device/sync/request", *msg);
  }
  
}

void handle_root_to_all_children(const app_packet_t *msg) {
  if (msg->msg_type == MSG_TYPE_RTC_SYNC) {
    if (!node_mesh_info.is_root) sync_rtc(msg->payload.timestamp);
  } else if (msg->msg_type == MSG_TYPE_MQTT_STATUS) {
    if (!node_mesh_info.is_root) sync_mqtt_status(msg->payload.mqtt_status_event.mqtt_status);
  } else if (msg->msg_type == MSG_TYPE_CONFIG_ALL_DEVICES) {
    ESP_LOGW(TAG, "CONFIG GLOBAL | ACCION: %s", msg->payload.accion);
  }
}

void handle_root_to_child(const app_packet_t *msg, const uint8_t *my_mac) {
  if (memcmp(msg->destination_mac, node_mesh_info.mac, 6) == 0) {
    if (msg->msg_type == MSG_TYPE_CONFIG_DEVICE) {
      ESP_LOGW(TAG, "CONFIG LOCAL | ACCION: %s", msg->payload.accion);
    }
  }
}