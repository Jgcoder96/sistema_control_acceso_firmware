#include "nvs_flash.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "nvs_init.h"
#include "nvs_sync_version.h"

static const char *TAG = "NVS_INIT";

void nvs_storage_init(void) {
  esp_err_t err = nvs_flash_init();
  if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
    ESP_ERROR_CHECK(nvs_flash_erase());
    err = nvs_flash_init();
  }
  ESP_ERROR_CHECK(err);
  ESP_LOGW(TAG, "[NVS] La memoria Flash NVS se ha inicializado correctamente.");
}

void nvs_init(void) {
  nvs_storage_init();
  xTaskCreate(nvs_sync_version, "nvs_sync_version", NVS_SYNC_VERSION_TASK_SIZE, NULL, NVS_SYNC_VERSION_TASK_PRIO, NULL);
}