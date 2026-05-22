static const char *TAG = "SISTEMA_ACCESO";
#include "nvs_get_version.h"
#include "nvs.h"
#include "esp_log.h"


void check_system_version() {
    char version_detectada[64] = {0}; 
    
    esp_err_t resultado = nvs_get_version(version_detectada, sizeof(version_detectada));

    if (resultado == ESP_OK) {
        // ESCENARIO: Ya existe una versión en memoria
        ESP_LOGI(TAG, "============================================");
        ESP_LOGI(TAG, "ESTA VERSION ES LA QUE TENGO: [%s]", version_detectada);
        ESP_LOGI(TAG, "¿ES LA MISMA QUE TIENES EN LA BASE DE DATOS?");
        ESP_LOGI(TAG, "============================================");
    } 
    else if (resultado == ESP_ERR_NVS_NOT_FOUND) {
        // ESCENARIO: No hay nada guardado (Primera vez)
        ESP_LOGW(TAG, "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
        ESP_LOGW(TAG, "AVISO: NO EXISTE VERSION GUARDADA.");
        ESP_LOGW(TAG, "POR FAVOR SINCRONIZAR.");
        ESP_LOGW(TAG, "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
    } 
    else {
        // ESCENARIO: Otro tipo de error (NVS no inicializado, etc.)
        ESP_LOGE(TAG, "ERROR AL ACCEDER A NVS: %s", esp_err_to_name(resultado));
    }
}