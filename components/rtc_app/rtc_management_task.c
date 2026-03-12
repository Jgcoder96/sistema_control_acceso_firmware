#include <stdbool.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

#include "rtc_get_time_from_module.h"
#include "rtc_i2c_config.h"
#include "rtc_set_time_from_ntp_server.h"
#include "rtc_sync_device_internal_clock.h"

static const char *TAG = "RTC_MANAGEMENT_TASK";

#define SYNC_INTERVAL_MS (24 * 60 * 60 * 1000) 

extern bool is_internet_available;
extern bool is_node_root;

void rtc_management_task(void *pvParameters) {
  uint32_t last_sync_tick = 0; 

  bool set_time_at_system_startup = true;

  rtc_i2c_config();

  rtc_sync_device_internal_clock();

  while (1) {
    uint32_t current_tick = xTaskGetTickCount() * portTICK_PERIOD_MS;
  
    if (is_internet_available && is_node_root) {

      if (set_time_at_system_startup || (current_tick - last_sync_tick >= SYNC_INTERVAL_MS)) {  
        ESP_LOGI(TAG, "Iniciando resincronización periódica del reloj con el servidor NTP...");
        if (set_time_from_npt_server()) {
          last_sync_tick = current_tick;
          set_time_at_system_startup = false;
          ESP_LOGI(TAG, "Sincronización del reloj exitosa. Próxima en 24 horas.");
        }
      }
    }
    rtc_get_time_from_module();
    vTaskDelay(pdMS_TO_TICKS(10000));
  }
}