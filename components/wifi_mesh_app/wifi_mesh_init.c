#include "esp_log.h"
#include "esp_mac.h"

#include "app_types.h"
#include "wifi_mesh_init.h"
#include "wifi_mesh_tasks.h"

static const char *TAG = "WIFI_MESH_INIT";

QueueHandle_t mesh_rx_queue = NULL;
QueueHandle_t mesh_tx_queue = NULL;

void wifi_mesh_init(void) {
  esp_read_mac(my_mac, ESP_MAC_WIFI_STA);

  mesh_rx_queue = xQueueCreate(MESH_RX_QUEUE_LENGTH, sizeof(mesh_packet_t));
  mesh_tx_queue = xQueueCreate(MESH_TX_QUEUE_LENGTH, sizeof(app_packet_t));

  xTaskCreate(mesh_receiver_task, "mesh_rx_task", MESH_RECEIVER_TASK_SIZE, NULL, MESH_RECEIVER_TASK_PRIO, NULL);
  xTaskCreate(mesh_transmitter_task, "mesh_tx_task", MESH_TRANSMITTER_TASK_SIZE, NULL, MESH_TRANSMITTER_TASK_PRIO, NULL);
  xTaskCreate(mesh_processor_task, "mesh_pd_task", MESH_PROCESSOR_TASK_SIZE, NULL, MESH_PROCESSOR_TASK_PRIO, NULL);

  ESP_LOGI(TAG, "Sistema Mesh Inicializado. MAC: " MACSTR, MAC2STR(my_mac));
}