#include "esp_event.h"
#include "esp_log.h"
#include "esp_mesh.h"
#include "esp_netif.h"
#include "esp_wifi.h"
#include "nvs_flash.h"
#include <string.h>

#include "config.h"
#include "wifi_mesh_event_handler.h"
#include "wifi_mesh_init.h"
#include "wifi_mesh_setup.h"

static const char *TAG = "WIFI_MESH_SETUP";

static void initialize_nvs_storage(void) {
  esp_err_t err = nvs_flash_init();
  if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
    ESP_ERROR_CHECK(nvs_flash_erase());
    err = nvs_flash_init();
  }
  ESP_ERROR_CHECK(err);
}

static uint8_t discover_router_primary_channel(void) {
  ESP_LOGI(TAG, "Iniciando escaneo de canal para SSID: %s...", WIFI_SSID);
    
  wifi_scan_config_t scan_config = {
    .ssid = (uint8_t *)WIFI_SSID,
    .show_hidden = true
  };
    
  esp_wifi_scan_start(&scan_config, true);
    
  uint16_t ap_count = 1;
  wifi_ap_record_t ap_record;
  uint8_t detected_channel = MESH_CHANNEL; 

  if (esp_wifi_scan_get_ap_records(&ap_count, &ap_record) == ESP_OK && ap_count > 0) {
    detected_channel = ap_record.primary;
    ESP_LOGI(TAG, "Router localizado con éxito. Operando en Canal: %d", detected_channel);
  } else {
    ESP_LOGW(TAG, "No se encontró el Router. Usando canal predeterminado: %d", detected_channel);
  }
  return detected_channel;
}

void wifi_mesh_setup(void) {
  // 1. Inicialización de persistencia
  initialize_nvs_storage();

  // 2. Inicialización de la pila TCP/IP y Lazo de Eventos
  ESP_ERROR_CHECK(esp_netif_init());
  ESP_ERROR_CHECK(esp_event_loop_create_default());
    
  // Crear interfaces de red por defecto para WiFi Mesh (STA y AP)
  esp_netif_t *netif_ap = NULL;
  ESP_ERROR_CHECK(esp_netif_create_default_wifi_mesh_netifs(&netif_sta, &netif_ap));

  // 3. Inicialización de Driver WiFi
  wifi_init_config_t wifi_config = WIFI_INIT_CONFIG_DEFAULT();
  ESP_ERROR_CHECK(esp_wifi_init(&wifi_config));
    
  // Suscripción a eventos críticos de Mesh e IP
  ESP_ERROR_CHECK(esp_event_handler_register(MESH_EVENT, ESP_EVENT_ANY_ID, &wifi_mesh_event_handler, NULL));
  ESP_ERROR_CHECK(esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &wifi_mesh_event_handler, NULL));
    
  ESP_ERROR_CHECK(esp_wifi_set_storage(WIFI_STORAGE_FLASH));
  ESP_ERROR_CHECK(esp_wifi_start());

  // 4. Sincronización de canal con el Punto de Acceso raíz
  uint8_t mesh_operating_channel = discover_router_primary_channel();

  // 5. Configuración del protocolo Mesh (MDF)
  ESP_ERROR_CHECK(esp_mesh_init());
  ESP_ERROR_CHECK(esp_mesh_set_max_layer(MESH_MAX_LAYER));
  ESP_ERROR_CHECK(esp_mesh_set_vote_percentage(1)); // 1.0 (100%) para votar por un nuevo Root

  mesh_cfg_t mesh_configuration = MESH_INIT_CONFIG_DEFAULT();
    
  // Configurar ID del Mesh (MAC compartida del grupo)
  uint8_t mesh_id_bytes[] = MESH_ID_BYTES;
  memcpy((uint8_t *) &mesh_configuration.mesh_id, mesh_id_bytes, 6);
    
  // Configurar canal y seguridad del AP interno del Mesh
  mesh_configuration.channel = mesh_operating_channel;
  memcpy(mesh_configuration.mesh_ap.password, MESH_PASSWORD, strlen(MESH_PASSWORD));
  mesh_configuration.mesh_ap.max_connection = 6;

  // Configurar credenciales para la conexión al Router (External Network)
  mesh_configuration.router.ssid_len = strlen(WIFI_SSID);
  memcpy(mesh_configuration.router.ssid, WIFI_SSID, strlen(WIFI_SSID));
  memcpy(mesh_configuration.router.password, WIFI_PWD, strlen(WIFI_PWD));

  // Establecer estado inicial y aplicar configuración
  ESP_ERROR_CHECK(esp_mesh_set_type(MESH_IDLE));
  ESP_ERROR_CHECK(esp_mesh_set_config(&mesh_configuration));
    
  // Arrancar el stack Mesh
  ESP_ERROR_CHECK(esp_mesh_start());
}