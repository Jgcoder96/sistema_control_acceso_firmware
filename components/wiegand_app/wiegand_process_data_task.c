/**
 * @file wiegand_process_data_task.c
 * @brief Implementación del procesamiento lógico de tarjetas y permisos.
 */
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "mqtt_client.h"
#include <inttypes.h>    
#include <string.h>     
#include <time.h>   

#include "esp_log.h"     
#include "nvs.h"       

#include "app_types.h"
#include "wiegand_types.h"
#include "wifi_mesh_transmission.h"
#include "wifi_mesh_info.h"
#include "rgb_led.h"
#include "hardware_control_task.h"
#include "nvs_offline_events.h"
#include "buzzer_app.h"
#include "display_app.h"

static const char *TAG = "WIEGAND_PROCESS_DATA_TASK";

extern QueueHandle_t wiegand_reader_queue;
extern QueueHandle_t hardware_control_queue;

static bool is_today_holiday(struct tm *now) {
  nvs_handle_t handle;
  size_t size;
  bool found = false;

  if (nvs_open("storage", NVS_READONLY, &handle) != ESP_OK) return false;

   
  if (nvs_get_blob(handle, "festivos", NULL, &size) == ESP_OK && size > 0) {
    uint8_t *data = malloc(size);
    if (nvs_get_blob(handle, "festivos", data, &size) == ESP_OK) {
      int num_festivos = size / sizeof(festivo_t);
      festivo_t *lista = (festivo_t *)data;

      // Buscar si el día de hoy coincide con algún festivo de la lista
      for (int i = 0; i < num_festivos; i++) {
        // Si el año en NVS es 0, aplica para todos los años
        bool year_match = (lista[i].anio == 0) || (lista[i].anio == (now->tm_year + 1900));
                
        if (lista[i].dia == now->tm_mday && lista[i].mes == (now->tm_mon + 1) && year_match) {
          found = true;
          break;
        }
      }
    }
    free(data);
  }
  nvs_close(handle);
  return found;
}

static bool validate_local_access(uint64_t full_id) {
  nvs_handle_t handle;
  char key[16];
  bool permitted = false;
  size_t size;

    
  time_t now_ts;
  time(&now_ts);
  struct tm timeinfo;
  localtime_r(&now_ts, &timeinfo);

  int dia_semana = (timeinfo.tm_wday == 0) ? 7 : timeinfo.tm_wday;
  bool hoy_es_festivo = is_today_holiday(&timeinfo);
    
  // El tipo 0 suele significar "Día Festivo" en la BD. Si no, es del 1 (Lunes) al 7 (Domingo).
  uint8_t dia_a_validar = hoy_es_festivo ? 0 : (uint8_t)dia_semana;

  ESP_LOGI(TAG, "Validando: %s | Día Sem: %d | ¿Festivo?: %s", hoy_es_festivo ? "FESTIVO" : "NORMAL", dia_semana, hoy_es_festivo ? "SI" : "NO");

  uint32_t card_id_32 = (uint32_t)full_id;
  snprintf(key, sizeof(key), "%lu", card_id_32);

  if (nvs_open("storage", NVS_READONLY, &handle) != ESP_OK) return false;

  if (nvs_get_blob(handle, key, NULL, &size) == ESP_OK) {
    uint8_t *buffer = malloc(size);
    if (nvs_get_blob(handle, key, buffer, &size) == ESP_OK) { 
      uint8_t num_reglas = buffer[0];
      regla_t *reglas = (regla_t *)(buffer + 1);

      for (int i = 0; i < num_reglas; i++) {
        // ¿Aplica para el día de hoy?
        if (reglas[i].dia_tipo == dia_a_validar) {
          // Convertir todo a segundos desde medianoche para comparar fácil
          uint32_t seg_ahora = (timeinfo.tm_hour * 3600) + (timeinfo.tm_min * 60) + timeinfo.tm_sec;
          uint32_t seg_inicio = (reglas[i].h_i * 3600) + (reglas[i].m_i * 60) + reglas[i].s_i;
          uint32_t seg_fin = (reglas[i].h_f * 3600) + (reglas[i].m_f * 60) + reglas[i].s_f;

          if (seg_ahora >= seg_inicio && seg_ahora <= seg_fin) {
            permitted = true;
            break; // Si hay al menos una regla que permita, ya puede entrar
          }
        }
      }
    }
    free(buffer);
  }
  nvs_close(handle);

  hardware_cmd_t action = { .permitted = permitted };
  if (hardware_control_queue != NULL) {
    xQueueSend(hardware_control_queue, &action, 0);
  }

  if (permitted) {
    ESP_LOGI(TAG, "✅ Acceso concedido a tarjeta %s", key);
  } else {
    ESP_LOGW(TAG, "❌ Acceso denegado a tarjeta %s (No reglas o fuera de horario)", key);
  }

  return permitted;
}

void wiegand_process_data_task(void *pvParameters) {
  wiegand_card_t card;
    
  while (1) {
    if (xQueueReceive(wiegand_reader_queue, &card, portMAX_DELAY) == pdTRUE) {
      buzzer_app_beep(2000, 100); // Pitido corto al leer la tarjeta
      
      bool access_granted = validate_local_access(card.full_id);
      
      // Mostrar en pantalla el resultado
      display_app_show_popup(access_granted);

      // Creamos el evento de acceso
      access_event_t event;
      event.timestamp = (uint32_t)time(NULL);
      event.access = access_granted;
      snprintf(event.card_id, sizeof(event.card_id), "%" PRIu64, card.full_id);

      // Decidimos: ¿Enviar o Guardar?
      if (node_mesh_info.is_mqtt_connected) {
          static app_packet_t data;
          memset(&data, 0, sizeof(data));
          memcpy(data.source_mac, node_mesh_info.mac, 6);
          data.msg_type = MSG_TYPE_CARD;
          data.payload.access_event = event;

          send_upstream(&data);
      } else {
          store_event_offline(&event);
      }
    }
  }
}