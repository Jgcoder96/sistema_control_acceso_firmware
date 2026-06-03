#include <stdint.h>     
#include <stddef.h>     

#include "esp_log.h"   

#include "app_types.h"  

static const char *TAG = "SHOW_PERMISSIONS_TO_SYNC";

void show_permissions_to_sync(const sync_data_event_t *sync_ev) {
   ESP_LOGI(TAG, "=== DATOS RECIBIDOS (V.%lu) ===", sync_ev->version);

  int num_festivos = sync_ev->festivos_len / sizeof(festivo_t);

  festivo_t *lista_f = (festivo_t *)sync_ev->data;

  for (int i = 0; i < num_festivos; i++) {
    ESP_LOGI(TAG, "📅 Festivo: %02d/%02d/%d", lista_f[i].dia, lista_f[i].mes, lista_f[i].anio);
  }

    
  uint8_t *p_ptr = (uint8_t *)(sync_ev->data + sync_ev->festivos_len);

  size_t offset = 0;

  while (offset < sync_ev->permisos_len) {
    uint32_t tarjeta_id = *(uint32_t *)(p_ptr + offset);
    uint8_t num_reglas = *(uint8_t *)(p_ptr + offset + 4);

    ESP_LOGI(TAG, "💳 Tarjeta ID: %lu | Reglas: %d", tarjeta_id, num_reglas);
        
    offset += 5;
    
    for (int r = 0; r < num_reglas; r++) {
      regla_t *regla = (regla_t *)(p_ptr + offset);
      ESP_LOGI(TAG, "   -> [%d] %02d:%02d:%02d a %02d:%02d:%02d", regla->dia_tipo, regla->h_i, regla->m_i, regla->s_i, regla->h_f, regla->m_f, regla->s_f);
            offset += sizeof(regla_t);
    }
  }
}