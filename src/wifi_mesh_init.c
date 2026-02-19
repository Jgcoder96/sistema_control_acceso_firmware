#include "esp_log.h"
#include "esp_mac.h"

#include "wifi_mesh_init.h"
#include "wifi_mesh_tasks.h"

static const char *TAG = "WIFI_MESH_INIT";

QueueHandle_t mesh_rx_queue = NULL;
QueueHandle_t mesh_tx_queue = NULL;

void wifi_mesh_init(void) {
  esp_read_mac(my_mac, ESP_MAC_WIFI_STA);

  mesh_rx_queue = xQueueCreate(RX_QUEUE_SIZE, sizeof(mesh_packet_t));
  mesh_tx_queue = xQueueCreate(RX_QUEUE_SIZE, sizeof(app_packet_t));

  xTaskCreate(mesh_receiver_task,"mesh_rx_task",3072, NULL,5,NULL);
  xTaskCreate(mesh_transmitter_task,"mesh_tx_task",3072, NULL,5,NULL);
  xTaskCreate(mesh_processor_task,"mesh_pd_task",3072,NULL,5,NULL);
    
  ESP_LOGI(TAG, "Sistema Mesh Inicializado. MAC: " MACSTR, MAC2STR(my_mac));
}