#include "nvs_offline_events.h"
#include "nvs_flash.h"
#include "esp_log.h"
#include <string.h>
#include <stdio.h>

static const char *TAG = "NVS_OFFLINE_STORAGE";
static const char *NVS_LOG_NS = "offline_logs";

typedef struct {
  uint16_t head;  
  uint16_t tail;  
  uint16_t count; 
} log_index_t;

static esp_err_t get_log_index(log_index_t *idx) {
  nvs_handle_t handle;
  size_t size = sizeof(log_index_t);
  memset(idx, 0, sizeof(log_index_t));
    
  esp_err_t err = nvs_open(NVS_LOG_NS, NVS_READONLY, &handle);
  if (err != ESP_OK) return err;
    
  err = nvs_get_blob(handle, "idx", idx, &size);
  nvs_close(handle);
  return err;
}

static esp_err_t save_log_index(log_index_t *idx) {
  nvs_handle_t handle;
  esp_err_t err = nvs_open(NVS_LOG_NS, NVS_READWRITE, &handle);
  if (err != ESP_OK) return err;
    
  err = nvs_set_blob(handle, "idx", idx, sizeof(log_index_t));
  if (err == ESP_OK) nvs_commit(handle);
    
  nvs_close(handle);
  return err;
}

esp_err_t init_offline_storage(void) {
  log_index_t idx;
  esp_err_t err = get_log_index(&idx);
  if (err == ESP_ERR_NVS_NOT_FOUND) {
    ESP_LOGI(TAG, "Inicializando índices por primera vez...");
    memset(&idx, 0, sizeof(log_index_t));
    return save_log_index(&idx);
  }
  ESP_LOGI(TAG, "[NVS] Almacenamiento offline cargado. Eventos pendientes: %u", idx.count);
  return ESP_OK;
}

esp_err_t store_event_offline(access_event_t *event) {
  log_index_t idx;
  get_log_index(&idx); 

  nvs_handle_t handle;
  esp_err_t err = nvs_open(NVS_LOG_NS, NVS_READWRITE, &handle);
  if (err != ESP_OK) return err;

  char key[12];
  snprintf(key, sizeof(key), "e_%u", idx.head);
    
  err = nvs_set_blob(handle, key, event, sizeof(access_event_t));
  if (err != ESP_OK) {
    nvs_close(handle);
    return err;
  }

  idx.head = (idx.head + 1) % MAX_OFFLINE_LOGS;
  if (idx.count < MAX_OFFLINE_LOGS) {
    idx.count++;
  } else {
    idx.tail = (idx.tail + 1) % MAX_OFFLINE_LOGS;
    ESP_LOGW(TAG, "[NVS] Memoria llena, sobreescribiendo evento antiguo.");
  }

  nvs_commit(handle);
  nvs_close(handle);
    
  save_log_index(&idx);
  ESP_LOGI(TAG, "[NVS] Evento guardado en NVS. Total: %u", idx.count);
  return ESP_OK;
}

esp_err_t pop_offline_event(access_event_t *out_event) {
  log_index_t idx;
  if (get_log_index(&idx) != ESP_OK || idx.count == 0) {
    return ESP_ERR_NOT_FOUND;
  }

  nvs_handle_t handle;
  esp_err_t err = nvs_open(NVS_LOG_NS, NVS_READWRITE, &handle);
  if (err != ESP_OK) return err;

  char key[12];
  snprintf(key, sizeof(key), "e_%u", idx.tail);
    
  size_t size = sizeof(access_event_t);
  err = nvs_get_blob(handle, key, out_event, &size);
    
  if (err == ESP_OK) {
    nvs_erase_key(handle, key); 
    idx.tail = (idx.tail + 1) % MAX_OFFLINE_LOGS;
    idx.count--;
    nvs_commit(handle);
  }

  nvs_close(handle);
  save_log_index(&idx);
  return err;
}

uint16_t get_offline_event_count(void) {
  log_index_t idx;
  if (get_log_index(&idx) == ESP_OK) {
    return idx.count;
  }
  return 0;
}