#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "wifi_mesh_info.h"

extern SemaphoreHandle_t sync_trigger_sem;

void sync_device_trigger(void) {
  node_mesh_info.is_synchronized = false;
    
  if (sync_trigger_sem != NULL) {
    xSemaphoreGive(sync_trigger_sem);
  }
}