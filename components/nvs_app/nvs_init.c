/**
 * @file nvs_init.c
 * @brief Implementación de la inicialización de memoria y colas.
 */
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "freertos/task.h"
#include "nvs_flash.h"
#include "nvs_offline_events.h"
#include "nvs_offline_sync_task.h"

#include "nvs_init.h"
#include "nvs_sync_version.h"

static const char *TAG = "NVS_INIT";

SemaphoreHandle_t sync_trigger_sem = NULL;

void nvs_sync_trigger(void) {
  if (sync_trigger_sem != NULL) {
    xSemaphoreGive(sync_trigger_sem);
  }
}

void nvs_storage_init(void) {
  esp_err_t err = nvs_flash_init();
  // Si la partición NVS está corrupta o pertenece a una versión vieja,
  // la borramos y la volvemos a inicializar desde cero.
  if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
    ESP_ERROR_CHECK(nvs_flash_erase());
    err = nvs_flash_init();
  }
  ESP_ERROR_CHECK(err);
  ESP_LOGW(TAG, "[NVS] La memoria Flash NVS se ha inicializado correctamente.");
}

void nvs_init(void) {
  nvs_storage_init();

  init_offline_storage();


  // Semáforo para despertar a nvs_sync_version cuando sea necesario
  if (sync_trigger_sem == NULL) {
    sync_trigger_sem = xSemaphoreCreateBinary();
  }

  // Tarea encargada de pedir al backend los permisos actualizados
  xTaskCreate(nvs_sync_version, "nvs_sync_version", NVS_SYNC_VERSION_TASK_SIZE, NULL, NVS_SYNC_VERSION_TASK_PRIO, NULL);
  // Tarea encargada de enviar a la red Mesh los eventos que ocurrieron offline
  xTaskCreate(offline_sync_task, "offline_sync_task", NVS_OFFLINE_EVENTS_TASK_SIZE, NULL, NVS_OFFLINE_EVENTS_TASK_PRIO, NULL);

  xSemaphoreGive(sync_trigger_sem);
}