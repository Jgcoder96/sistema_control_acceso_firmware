#include "cJSON.h"
#include "esp_log.h"
#include <string.h> 
#include "mbedtls/base64.h"

#include "wifi_mesh_tasks.h"
#include "wifi_mesh_transmission.h"
#include "wifi_mesh_node_logic.h"
#include "app_types.h"
#include "wifi_mesh_info.h"

static const char *TAG = "MQTT_SUBSCRIPTION_MANAGER";

void subscription_manager_configure_device(cJSON *root) {
  cJSON *mac = cJSON_GetObjectItemCaseSensitive(root, "mac");
  cJSON *config = cJSON_GetObjectItemCaseSensitive(root, "config");
  cJSON *accion = (config != NULL) ? cJSON_GetObjectItemCaseSensitive(config, "accion") : NULL;

  if (cJSON_IsString(mac) && cJSON_IsString(accion)) {
    uint8_t temp_dest_mac[6];
    
    int mac_bytes = sscanf(mac->valuestring, "%02hhx:%02hhx:%02hhx:%02hhx:%02hhx:%02hhx", 
                        &temp_dest_mac[0], &temp_dest_mac[1], 
                        &temp_dest_mac[2], &temp_dest_mac[3], 
                        &temp_dest_mac[4], &temp_dest_mac[5]);

    if (mac_bytes != 6) {
        ESP_LOGE(TAG, "[MQTT] Formato de MAC inválido");
        return;
    }
    app_packet_t app_packet;    
    app_packet.msg_type = MSG_TYPE_CONFIG_DEVICE; 
        
    memcpy(app_packet.source_mac, node_mesh_info.mac, 6);      
    memcpy(app_packet.destination_mac, temp_dest_mac, 6);

    memset(app_packet.payload.accion, 0, sizeof(app_packet.payload.accion));
    strncpy(app_packet.payload.accion, accion->valuestring, sizeof(app_packet.payload.accion) - 1);

    if (memcmp(temp_dest_mac, node_mesh_info.mac, 6) == 0) {
      handle_root_to_child(&app_packet, node_mesh_info.mac);
    } else {
      send_downstream(&app_packet);
    }
    
  } else {
    ESP_LOGW(TAG, "[MQTT] Error al configurar el dispositivo: datos incompletos");
  }
}

void subscription_manager_configure_all_devices(cJSON *root) {
  cJSON *config = cJSON_GetObjectItemCaseSensitive(root, "config");
  cJSON *accion = (config != NULL) ? cJSON_GetObjectItemCaseSensitive(config, "accion") : NULL;

  if (cJSON_IsString(accion)) {
    app_packet_t app_packet;
    app_packet.msg_type = MSG_TYPE_CONFIG_ALL_DEVICES; 
    memcpy(app_packet.source_mac, node_mesh_info.mac, 6);   
        
    memset(app_packet.destination_mac, 0xFF, 6);
        
    memset(app_packet.payload.accion, 0, sizeof(app_packet.payload.accion));
    strncpy(app_packet.payload.accion, accion->valuestring, sizeof(app_packet.payload.accion) - 1);     
        
    broadcast_to_mesh(&app_packet);
    } else {
      ESP_LOGW(TAG, "[MQTT] Error al configurar el dispositivo");
    }
}


void procesar_mensaje_sincronizacion(cJSON *root) {
    cJSON *version = cJSON_GetObjectItemCaseSensitive(root, "version");
    cJSON *mac_b64 = cJSON_GetObjectItemCaseSensitive(root, "mac");

    if (!cJSON_IsNumber(version) || !cJSON_IsString(mac_b64)) {
      ESP_LOGE(TAG, "JSON mal formado: faltan campos 'version' o 'mac'");
      return;
    }

    uint8_t mac_recibida_bin[6];
    size_t out_len;
    
    int ret = mbedtls_base64_decode(mac_recibida_bin, sizeof(mac_recibida_bin), &out_len, 
                                   (const unsigned char *)mac_b64->valuestring, 
                                   strlen(mac_b64->valuestring));

    if (ret != 0 || out_len != 6) {
      ESP_LOGE(TAG, "Error al decodificar la MAC binaria del JSON");
      return;
    }

    if (memcmp(mac_recibida_bin, node_mesh_info.mac, 6) == 0) {
        
        
      ESP_LOGI(TAG, "--------------------------------------------------");
      ESP_LOGI(TAG, "🏠 ¡COINCIDENCIA! El mensaje es para el NODO ROOT");
      ESP_LOGI(TAG, "Versión recibida: %d", version->valueint);
        
        
      char *json_render = cJSON_Print(root);
      printf("%s\n", json_render);
      free(json_render);
        
      ESP_LOGI(TAG, "--------------------------------------------------");

    } else {
      ESP_LOGW(TAG, "--------------------------------------------------");
      ESP_LOGW(TAG, "📡 AVISO: El mensaje NO es para mí (ROOT)");
      ESP_LOGW(TAG, "Destinado al Nodo MAC: %02X:%02X:%02X:%02X:%02X:%02X",
                mac_recibida_bin[0], mac_recibida_bin[1], mac_recibida_bin[2],
                mac_recibida_bin[3], mac_recibida_bin[4], mac_recibida_bin[5]);
        
        
      char *json_render = cJSON_Print(root);
      printf("Contenido a rutear:\n%s\n", json_render);
      free(json_render);
        
      ESP_LOGW(TAG, "--------------------------------------------------");
    }
}