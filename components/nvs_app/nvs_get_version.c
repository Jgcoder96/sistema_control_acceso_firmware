/**
 * @file nvs_get_version.c
 * @brief Extrae la versión sincronizada actual.
 */
#include "nvs_flash.h"
#include "nvs.h"

//static const char *TAG = "NVS_GET_VERSION";

esp_err_t nvs_get_version(char* version, size_t length) {
  nvs_handle_t my_handle;
  esp_err_t err;

  err = nvs_open("storage", NVS_READONLY, &my_handle);
  if (err != ESP_OK) {
    return err; 
  }

  // Leer la cadena que corresponde a la llave sync_ver
  err = nvs_get_str(my_handle, "sync_ver", version, &length);
    
  nvs_close(my_handle);
  
  return err;
}
