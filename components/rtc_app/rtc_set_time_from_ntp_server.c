/**
 * @file rtc_set_time_from_ntp_server.c
 * @brief Implementación de la solicitud de hora mediante NTP.
 */
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
  ESP_LOGI(TAG, "[RTC] Iniciando sincronización con servidores NTP...");
     
  if (esp_sntp_enabled()) {
    esp_sntp_stop();
  }

  // Configurar los servidores NTP (Principal y Respaldo)
  esp_sntp_setoperatingmode(SNTP_OPMODE_POLL);
  esp_sntp_setservername(0, NTP_SERVER);        
  esp_sntp_setservername(1, NTP_SERVER_BACKUP); 
    
  esp_sntp_init();

  int retry = 0;
  sntp_sync_status_t status = SNTP_SYNC_STATUS_RESET;

  // Bucle de espera bloqueante hasta obtener la hora o agotar reintentos
  while (status == SNTP_SYNC_STATUS_RESET && ++retry <= NTP_MAX_RETRY) {
    ESP_LOGI(TAG, "[RTC] Conectando a NTP... Intento (%d/%d)", retry, NTP_MAX_RETRY);
    vTaskDelay(pdMS_TO_TICKS(NTP_RETRY_DELAY_MS));
    status = sntp_get_sync_status();
  }

  if (status == SNTP_SYNC_STATUS_COMPLETED) {

    time_t now;
    struct tm timeinfo;
    time(&now);
    localtime_r(&now, &timeinfo);

    // Validación extra para ignorar fechas erróneas devueltas por un servidor caído (ej. año 1970)
    if (timeinfo.tm_year < (2024 - 1900)) {
      ESP_LOGE(TAG, "[RTC] Error: Hora inválida.");
      esp_sntp_stop();
      return false;
    }

    // Si la hora es coherente, respaldarla inmediatamente en el hardware (DS3231)    
    if (rtc_write_time_in_module(&timeinfo) == ESP_OK) {
      ESP_LOGI(TAG, "[RTC] Sincronización total exitosa.");
      ESP_LOGI(TAG, "[RTC] Hora aplicada: %02d:%02d:%02d",timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec);
      esp_sntp_stop();
      return true;
    } else {
      ESP_LOGE(TAG, "[RTC] NTP OK, pero fallo al escribir en el chip físico por I2C.");
    }
  } else {
    ESP_LOGE(TAG, "[RTC] Fallo: No se pudo contactar con ningún servidor NTP.");
  }
  
    esp_sntp_stop();
    return false;
}