#include "cJSON.h"
#include "esp_log.h"
#include <string.h> 
#include "mbedtls/base64.h"

#include "app_types.h"
#include "wifi_mesh_info.h"
#include "wifi_mesh_node_logic.h"
#include "wifi_mesh_tasks.h"
#include "wifi_mesh_transmission.h"
#include "show_permissions_to_sync.h"
#include "nvs_save_permissions.h"
#include "sync_device_trigger.h"


static const char *TAG = "MQTT_SUBSCRIPTION_MANAGER";

void procesar_mensaje_sincronizacion(cJSON *root) {
  if (root == NULL) return;
  cJSON *item = root;
  if (cJSON_IsArray(root)) {
    item = cJSON_GetArrayItem(root, 0);
  }

  cJSON *version = cJSON_GetObjectItemCaseSensitive(item, "version");
  cJSON *mac_b64 = cJSON_GetObjectItemCaseSensitive(item, "mac");
  cJSON *festivos_b64 = cJSON_GetObjectItemCaseSensitive(item, "festivos");
  cJSON *permisos_b64 = cJSON_GetObjectItemCaseSensitive(item, "permisos_nvs");

  if (!cJSON_IsNumber(version) || !cJSON_IsString(mac_b64) || !cJSON_IsString(permisos_b64)) {
    ESP_LOGE(TAG, "JSON incompleto: faltan campos críticos");
    return;
  }

  uint8_t target_mac[6];
  size_t out_len_mac;
  mbedtls_base64_decode(target_mac, sizeof(target_mac), &out_len_mac, (const unsigned char *)mac_b64->valuestring, strlen(mac_b64->valuestring));

  if (out_len_mac != 6) {
    ESP_LOGE(TAG, "Error: MAC decodificada tiene longitud inválida");
    return;
  }

   
  app_packet_t packet;
  memset(&packet, 0, sizeof(app_packet_t));

  packet.msg_type = MSG_TYPE_SYNC_DATA;
  memcpy(packet.source_mac, node_mesh_info.mac, 6);
  memcpy(packet.destination_mac, target_mac, 6);

    
  sync_data_event_t *sync_ev = &packet.payload.sync_data_event;
  sync_ev->version = (uint32_t)version->valueint;

  size_t f_bytes = 0;
  if (cJSON_IsString(festivos_b64)) {
    mbedtls_base64_decode(sync_ev->data, sizeof(sync_ev->data), &f_bytes, (const unsigned char *)festivos_b64->valuestring, strlen(festivos_b64->valuestring));
  }

  sync_ev->festivos_len = (uint16_t)f_bytes;

    
  size_t p_bytes = 0;
  size_t space_left = sizeof(sync_ev->data) - f_bytes;
    
  mbedtls_base64_decode(sync_ev->data + f_bytes, space_left, &p_bytes, (const unsigned char *)permisos_b64->valuestring, strlen(permisos_b64->valuestring));
    
  sync_ev->permisos_len = (uint16_t)p_bytes;

    
  if (memcmp(target_mac, node_mesh_info.mac, 6) == 0) {
     ESP_LOGI(TAG, "🏠 COINCIDENCIA: Procesando Sync Local (ROOT)");

    show_permissions_to_sync(sync_ev);
    nvs_save_permissions(sync_ev);

  } else {
    ESP_LOGW(TAG, "📡 RUTEANDO: Enviando paquete binario a un hijo.");
    packet.type = ROOT_TO_CHILD;
    send_downstream(&packet); 
  }
}

void subscription_manager_sync_trigger(cJSON *root) {
  if (root == NULL) return;

  cJSON *item = cJSON_IsArray(root) ? cJSON_GetArrayItem(root, 0) : root;

    
  cJSON *mac_b64 = cJSON_GetObjectItemCaseSensitive(item, "mac");
  cJSON *execute = cJSON_GetObjectItemCaseSensitive(item, "execute");

  if (!cJSON_IsString(mac_b64) || !cJSON_IsBool(execute)) {
    ESP_LOGE(TAG, "Gatillo inválido: faltan campos 'mac' o 'execute'");
    return;
  }

    
  if (!cJSON_IsTrue(execute)) {
    return;
  }

    
  uint8_t target_mac[6];
  size_t out_len;
  int ret = mbedtls_base64_decode(target_mac, 6, &out_len, (const unsigned char *)mac_b64->valuestring, strlen(mac_b64->valuestring));

  if (ret != 0 || out_len != 6) {
    ESP_LOGE(TAG, "Error decodificando MAC binaria");
    return;
  }

  app_packet_t packet;
  memset(&packet, 0, sizeof(app_packet_t));

  packet.msg_type = MSG_TYPE_SYNC_TRIGGER;
  memcpy(packet.source_mac, node_mesh_info.mac, 6);
  memcpy(packet.destination_mac, target_mac, 6);
    
    
  packet.payload.sync_trigger_event.execute = true;

    
  if (memcmp(target_mac, node_mesh_info.mac, 6) == 0) {  
    ESP_LOGW(TAG, "🏠 Coincidencia: Activando sincronización local en ROOT");
    sync_device_trigger();

  } else {
    ESP_LOGI(TAG, "📡 Ruteando gatillo al hijo: %02X:%02X:%02X:%02X:%02X:%02X",
              target_mac[0], target_mac[1], target_mac[2], 
              target_mac[3], target_mac[4], target_mac[5]);
        
    packet.type = ROOT_TO_CHILD;
    send_downstream(&packet);
        
    ESP_LOGI(TAG, "✅ Gatillo enviado a la malla.");
  }
}