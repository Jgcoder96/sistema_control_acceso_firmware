#include "nvs_flash.h"
#include "esp_log.h"

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