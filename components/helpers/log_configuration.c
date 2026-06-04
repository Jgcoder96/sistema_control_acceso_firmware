#include "esp_log.h"

void log_configuration() {
  esp_log_level_set("*", ESP_LOG_WARN); 

  esp_log_level_set("wifi", ESP_LOG_NONE);
  esp_log_level_set("mesh", ESP_LOG_NONE);

  esp_log_level_set("SHOW_PERMISSIONS_TO_SYNC", ESP_LOG_INFO);

  esp_log_level_set("MQTT_MANAGER", ESP_LOG_INFO);
  esp_log_level_set("MQTT_PUBLISHER", ESP_LOG_INFO);
  esp_log_level_set("MQTT_STATUS_BROADCAST", ESP_LOG_INFO);
  esp_log_level_set("MQTT_SUBSCRIBER", ESP_LOG_INFO);
  esp_log_level_set("MQTT_SUBSCRIPTION_MANAGER", ESP_LOG_INFO);

  esp_log_level_set("SYNC_MQTT_STATUS", ESP_LOG_INFO);
  esp_log_level_set("SYNC_RTC", ESP_LOG_INFO);

  esp_log_level_set("NVS_GET_VERSION", ESP_LOG_INFO);
  esp_log_level_set("NVS_INIT", ESP_LOG_INFO);
  esp_log_level_set("NVS_SYNC_PERMISSIONS", ESP_LOG_INFO);
  esp_log_level_set("NVS_SYNC_VERSION", ESP_LOG_INFO);

  
  esp_log_level_set("RTC_GET_TIME_FROM_MODULE", ESP_LOG_INFO);
  esp_log_level_set("RTC_MANAGEMENT_TASK", ESP_LOG_INFO);
  esp_log_level_set("RTC_SET_TIME_FROM_NTP_SERVER", ESP_LOG_INFO);

  esp_log_level_set("WIEGAND_INIT", ESP_LOG_INFO);

  esp_log_level_set("RSSI_MONITOR", ESP_LOG_INFO);
  esp_log_level_set("WIFI_MESH_EVENT_HANDLER", ESP_LOG_INFO);
  esp_log_level_set("WIFI_MESH_INIT", ESP_LOG_INFO);
  esp_log_level_set("WIFI_MESH_NODE_LOGIC", ESP_LOG_INFO);
  esp_log_level_set("WIFI_MESH_SETUP", ESP_LOG_INFO);
  esp_log_level_set("WIFI_MESH_TASKS", ESP_LOG_INFO);


}