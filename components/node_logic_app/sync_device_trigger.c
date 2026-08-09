/**
 * @file sync_device_trigger.c
 * @brief Implementación del trigger de sincronización manual.
 */
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "wifi_mesh_info.h"

extern SemaphoreHandle_t sync_trigger_sem;

void sync_device_trigger(void) {
  // Invalidar el estado actual de sincronización para forzar la recarga
  node_mesh_info.is_synchronized = false;
    
  // Desbloquear la tarea que envía la petición a través de Mesh
  if (sync_trigger_sem != NULL) {
    xSemaphoreGive(sync_trigger_sem);
  }
}