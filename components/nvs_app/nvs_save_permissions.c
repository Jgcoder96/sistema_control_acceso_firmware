#include <stdio.h>     
#include <stdint.h>     
#include <string.h>    

#include "nvs.h"        
#include "nvs_flash.h" 
#include "esp_log.h"   
#include "esp_err.h"   

#include "app_types.h"
#include "wifi_mesh_info.h"
#include "wifi_mesh_transmission.h"
#include "mqtt_publisher.h"

static const char *TAG = "NVS_SYNC_PERMISSIONS";

static void notify_server_of_successful_save(uint32_t version, bool exito) {
  app_packet_t ack_packet;
  memset(&ack_packet, 0, sizeof(app_packet_t));

  ack_packet.msg_type = MSG_TYPE_SYNC_ACK;
    
  memcpy(ack_packet.source_mac, node_mesh_info.mac, 6);
    
  ack_packet.payload.sync_ack_event.version = version;
    
  ack_packet.payload.sync_ack_event.success = exito;
    

  if (node_mesh_info.is_root) {
    mqtt_publisher("device/sync/successful", ack_packet);
  } else {
    ack_packet.type = CHILD_TO_ROOT;
    send_upstream(&ack_packet);
  }
}


void nvs_save_permissions(const sync_data_event_t *sync_ev) {
  nvs_handle_t handle;
  esp_err_t err;

  err = nvs_open("storage", NVS_READWRITE, &handle);
  if (err != ESP_OK) {
    ESP_LOGE(TAG, "Error abriendo NVS: %s", esp_err_to_name(err));
    return;
  }

  uint32_t version_actual = 0;
  
  nvs_get_u32(handle, "sync_ver", &version_actual);

  if (version_actual == sync_ev->version) {
    ESP_LOGI(TAG, "Versión %lu ya está al día. Saltando escritura.", version_actual);
    nvs_close(handle);
    node_mesh_info.is_synchronized = true; 
    notify_server_of_successful_save(sync_ev->version, true);
    return;
  }

  ESP_LOGW(TAG, "Nueva Versión (%lu -> %lu). Limpiando NVS...", version_actual, sync_ev->version);
  nvs_erase_all(handle);

  
  nvs_set_u32(handle, "sync_ver", sync_ev->version);

  
  if (sync_ev->festivos_len > 0) {
    nvs_set_blob(handle, "festivos", sync_ev->data, sync_ev->festivos_len);
    ESP_LOGI(TAG, "✅ Festivos actualizados.");
  }

    
  uint8_t *p_ptr = (uint8_t *)(sync_ev->data + sync_ev->festivos_len);
  size_t offset = 0;
  int contador = 0;

  while (offset < sync_ev->permisos_len) {
    uint32_t tarjeta_id = *(uint32_t *)(p_ptr + offset);
    uint8_t num_reglas = *(uint8_t *)(p_ptr + offset + 4);
        
    size_t tam_reglas = 1 + (num_reglas * sizeof(regla_t));

    char key[16];
    snprintf(key, sizeof(key), "%lu", tarjeta_id);

        
    nvs_set_blob(handle, key, (p_ptr + offset + 4), tam_reglas);
        
    offset += 4 + tam_reglas;
    contador++;
  }

  nvs_commit(handle);
  nvs_close(handle);

  node_mesh_info.is_synchronized = true;

  notify_server_of_successful_save(sync_ev->version, true);
  
  ESP_LOGI(TAG, "💾 Sincronización exitosa. %d tarjetas guardadas.", contador);
}