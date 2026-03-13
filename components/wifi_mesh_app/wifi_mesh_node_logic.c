#include <sys/time.h>  
#include <inttypes.h>   
#include "esp_log.h"

#include "app_types.h"
#include "mqtt_publisher.h"
#include "rtc_write_time_in_module.h"

static const char *TAG = "WIFI_MESH_NODE_LOGIC";

void handle_child_to_root(const app_packet_t *msg) {
  mqtt_publisher("mesh/config", *msg);
}

void handle_root_to_all_children(const app_packet_t *msg) {
  if (msg->msg_type == MSG_TYPE_RTC_SYNC) {
    uint32_t ts_utc = msg->payload.timestamp;
    time_t t_utc = (time_t)ts_utc;
    struct tm timeinfo;

    struct timeval tv = { .tv_sec = t_utc, .tv_usec = 0 };
    settimeofday(&tv, NULL); 

    localtime_r(&t_utc, &timeinfo);

    esp_err_t err = rtc_write_time_in_module(&timeinfo);
        
    if (err == ESP_OK) {
      char buf[32];
      strftime(buf, sizeof(buf), "%d/%m/%Y %H:%M:%S", &timeinfo);
      ESP_LOGI(TAG, "Sincronización Exitosa (Hora Venezuela): %s", buf);
    } else {
      ESP_LOGE(TAG, "Error al escribir en DS1307");
    }
  } else if (msg->msg_type == MSG_TYPE_CONFIG_ALL_DEVICES) {
    ESP_LOGW(TAG, "CONFIG GLOBAL | ACCION: %s", msg->payload.accion);
  }
}

void handle_root_to_child(const app_packet_t *msg, const uint8_t *my_mac) {
  if (memcmp(msg->destination_mac, my_mac, 6) == 0) {
    if (msg->msg_type == MSG_TYPE_CONFIG_DEVICE) {
      ESP_LOGW(TAG, "CONFIG LOCAL | ACCION: %s", msg->payload.accion);
    }
  }
}