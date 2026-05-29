#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "esp_wifi.h"
#include "wifi_mesh_info.h"

//static const char *TAG = "RSSI_MONITOR";

void monitor_rssi_task(void *arg) {
  int rssi = 0;
    
  while (1) {
    esp_err_t err = esp_wifi_sta_get_rssi(&rssi);
        
    if (err == ESP_OK) {  
      node_mesh_info.rssi = rssi; 
    } else {
      node_mesh_info.rssi = 0;
    }
     
    vTaskDelay(pdMS_TO_TICKS(10000)); 
  }
}