#include "esp_log.h"
#include "esp_mac.h"

#include "app_types.h"
#include "wifi_mesh_init.h"
#include "wifi_mesh_tasks.h"
#include "wifi_mesh_monitor_rssi_task.h"
#include "wifi_mesh_config.h"
#include "wifi_mesh_info.h"

static const char *TAG = "WIFI_MESH_INIT";

QueueHandle_t mesh_rx_queue = NULL;
QueueHandle_t mesh_tx_queue = NULL;

node_mesh_info_t node_mesh_info;

void wifi_mesh_init(void) {
  esp_read_mac(node_mesh_info.mac, ESP_MAC_WIFI_STA);
  mesh_cfg_t cfg;
  esp_mesh_get_config(&cfg);
  memcpy(node_mesh_info.mesh_id.addr, cfg.mesh_id.addr, 6);

  mesh_rx_queue = xQueueCreate(MESH_RX_QUEUE_LENGTH, sizeof(mesh_packet_t));
  mesh_tx_queue = xQueueCreate(MESH_TX_QUEUE_LENGTH, sizeof(app_packet_t));

  xTaskCreate(mesh_receiver_task, "mesh_rx_task", MESH_RECEIVER_TASK_SIZE, NULL, MESH_RECEIVER_TASK_PRIO, NULL);
  xTaskCreate(mesh_transmitter_task, "mesh_tx_task", MESH_TRANSMITTER_TASK_SIZE, NULL, MESH_TRANSMITTER_TASK_PRIO, NULL);
  xTaskCreate(mesh_processor_task, "mesh_pd_task", MESH_PROCESSOR_TASK_SIZE, NULL, MESH_PROCESSOR_TASK_PRIO, NULL);
  xTaskCreate(monitor_rssi_task, "mesh_rssi_task", MESH_MONITOR_RSSI_TASK_SIZE, NULL, MESH_MONITOR_RSSI_TASK_PRIO, NULL);

  ESP_LOGI(TAG, "[WIFI_MESH] Sistema Mesh Inicializado. MAC: " MACSTR ", Mesh ID: " MACSTR, MAC2STR(node_mesh_info.mac), MAC2STR(node_mesh_info.mesh_id.addr));
}