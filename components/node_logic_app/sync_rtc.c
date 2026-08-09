/**
 * @file sync_rtc.c
 * @brief Implementación de la sincronización RTC.
 */
#include "esp_log.h"
#include "rtc_write_time_in_module.h"
#include <inttypes.h>   
#include <sys/time.h>  

static const char *TAG = "SYNC_RTC";

esp_err_t sync_rtc(uint32_t timestamp_utc) {
  time_t t_utc = (time_t)timestamp_utc;
  struct tm timeinfo;

  // Actualizar el reloj interno del sistema operativo (ESP32)
  struct timeval tv = { .tv_sec = t_utc, .tv_usec = 0 };
  if (settimeofday(&tv, NULL) != 0) {
    return ESP_FAIL;
  }

  // Convertir a estructura tm para extraer día, mes, año, hora, etc.
  localtime_r(&t_utc, &timeinfo);

  // Escribir la nueva fecha y hora por bus I2C hacia el chip DS3231
  esp_err_t err = rtc_write_time_in_module(&timeinfo);
        
  if (err == ESP_OK) {
    char buf[32];
    strftime(buf, sizeof(buf), "%d/%m/%Y %H:%M:%S", &timeinfo);
    ESP_LOGI(TAG, " [NODE] Sincronización módulo RTC OK: %s", buf);
  } else {
    ESP_LOGE(TAG, " [NODE] Error: Sincronización módulo RTC");
  }

  return err;
}