#include "nvs_flash.h"
#include "nvs.h"

esp_err_t nvs_get_version(char* version_out, size_t max_len) {
    nvs_handle_t my_handle;
    esp_err_t err;

    // Abrimos el espacio de nombres "storage" en modo solo lectura
    err = nvs_open("storage", NVS_READONLY, &my_handle);
    if (err != ESP_OK) {
        return err; 
    }

    // Intentamos leer el valor de la clave "sync_ver"
    err = nvs_get_str(my_handle, "sync_ver", version_out, &max_len);
    
    // Siempre cerrar el handle
    nvs_close(my_handle);
    return err;
}
