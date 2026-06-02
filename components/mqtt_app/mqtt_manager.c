#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "freertos/task.h"
#include "mqtt_client.h"

#include "mqtt_config.h"
#include "mqtt_manager.h"
#include "mqtt_subscriber.h"
#include "wifi_mesh_info.h"
#include "mqtt_status_broadcast_task.h"
#include "nvs_init.h"

static const char *TAG = "MQTT_MANAGER";

extern QueueHandle_t mqtt_subscription_queue;

bool mqtt_is_started = false;

esp_mqtt_client_handle_t client = NULL;

static void mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data) {
  esp_mqtt_event_handle_t event = event_data;
  switch ((esp_mqtt_event_id_t)event_id) {
    case MQTT_EVENT_CONNECTED:
        esp_mqtt_client_subscribe(event->client, SUBSCRIBE_IN_TOPIC_CONFIG_DEVICE, 1);
        esp_mqtt_client_subscribe(event->client, SUBSCRIBE_IN_TOPIC_CONFIG_ALL_DEVICE, 1);
        esp_mqtt_client_subscribe(event->client, SUBSCRIBE_IN_TOPIC_SYNC_RESPONSE, 1);
        break;

    case MQTT_EVENT_DATA: {
      mqtt_received_data_t *msg = malloc(sizeof(mqtt_received_data_t));
      if (msg == NULL) return;

      msg->data = malloc(event->data_len + 1);
      if (msg->data == NULL) {
        free(msg);
        return;
      }

      int t_len = (event->topic_len >= 64) ? 63 : event->topic_len;
      memcpy(msg->topic, event->topic, t_len);
      msg->topic[t_len] = '\0';

      memcpy(msg->data, event->data, event->data_len);
      msg->data[event->data_len] = '\0'; 
      msg->data_len = event->data_len;

    
      if (xQueueSend(mqtt_subscription_queue, &msg, 0) != pdPASS) {
        free(msg->data);
        free(msg);
      }
      break;
    }

    default:
      break;
  }
}

void mqtt_management_task(void *pvParameters) {
  esp_mqtt_client_config_t mqtt_cfg = {
    .broker.address.uri = MQTT_BROKER_URL,
  };

  client = esp_mqtt_client_init(&mqtt_cfg);
  esp_mqtt_client_register_event(client, ESP_EVENT_ANY_ID, mqtt_event_handler, NULL);

  while (1) {
    bool condition_met = (node_mesh_info.is_internet_available && node_mesh_info.is_root);
      if (condition_met && !mqtt_is_started) {
        ESP_LOGI(TAG, "[MQTT] Dispositivo elegido como root y cuenta con conexión a Internet. Iniciando MQTT...");
        esp_mqtt_client_start(client);

        mqtt_is_started = true;
        node_mesh_info.is_mqtt_connected = true;
        nvs_sync_trigger();

        if (node_mesh_info.is_root == true) send_mqtt_status_update();
        ESP_LOGI(TAG, "[MQTT] Conectado a MQTT.");
      } else if (!condition_met && mqtt_is_started) {
        ESP_LOGW(TAG, "[MQTT] El dispositivo ha perdido el root o la conexión a Internet. Deteniendo MQTT...");
        esp_mqtt_client_stop(client);
        mqtt_is_started = false;
        node_mesh_info.is_mqtt_connected = false;
        if (node_mesh_info.is_root == true) send_mqtt_status_update();
        
      }
      vTaskDelay(pdMS_TO_TICKS(2000));
    }
}