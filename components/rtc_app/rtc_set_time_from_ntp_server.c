#include <time.h>
#include <sys/time.h>
#include <stdlib.h>        
#include "esp_sntp.h"     
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "rtc_config.h"
#include "rtc_write_time_in_module.h"

static const char *TAG = "RTC_SET_TIME_FROM_NTP_SERVER";

bool set_time_from_npt_server() {
  ESP_LOGI(TAG, "Iniciando sincronización con servidores NTP...");
     
  if (esp_sntp_enabled()) {
    esp_sntp_stop();
  }

  esp_sntp_setoperatingmode(SNTP_OPMODE_POLL);
  esp_sntp_setservername(0, NTP_SERVER);        
  esp_sntp_setservername(1, NTP_SERVER_BACKUP); 
    
  esp_sntp_init();

  int retry = 0;
  sntp_sync_status_t status = SNTP_SYNC_STATUS_RESET;

  while (status == SNTP_SYNC_STATUS_RESET && ++retry <= NTP_MAX_RETRY) {
    ESP_LOGI(TAG, "Conectando a NTP... Intento (%d/%d)", retry, NTP_MAX_RETRY);
    vTaskDelay(pdMS_TO_TICKS(NTP_RETRY_DELAY_MS));
    status = sntp_get_sync_status();
  }

  if (status == SNTP_SYNC_STATUS_COMPLETED) {
    setenv("TZ", DEFAULT_TIMEZONE, 1);
    tzset();

    time_t now;
    struct tm timeinfo;
    time(&now);
    localtime_r(&now, &timeinfo);

    if (timeinfo.tm_year < (2024 - 1900)) {
      ESP_LOGE(TAG, "Error: Hora inválida.");
      esp_sntp_stop();
      return false;
    }

        
    if (rtc_write_time_in_module(&timeinfo) == ESP_OK) {
      ESP_LOGI(TAG, "Sincronización total exitosa.");
      ESP_LOGI(TAG, "Hora aplicada: %02d:%02d:%02d",timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec);
      esp_sntp_stop();
      return true;
    } else {
      ESP_LOGE(TAG, "NTP OK, pero fallo al escribir en el chip físico por I2C.");
    }
  } else {
    ESP_LOGE(TAG, "Fallo: No se pudo contactar con ningún servidor NTP.");
  }
  
    esp_sntp_stop();
    return false;
}