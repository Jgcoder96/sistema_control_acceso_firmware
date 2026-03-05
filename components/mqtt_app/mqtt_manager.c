#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "freertos/task.h"
#include "mqtt_client.h"

#include "mqtt_config.h"
#include "mqtt_manager.h"
#include "mqtt_subscriber.h"

static const char *TAG = "MQTT_MANAGER";

extern bool is_internet_available;
extern bool is_node_root;
extern QueueHandle_t mqtt_subscription_queue;

bool mqtt_is_started = false;
esp_mqtt_client_handle_t client = NULL;

static void mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data) {
  esp_mqtt_event_handle_t event = event_data;
  switch ((esp_mqtt_event_id_t)event_id) {
    case MQTT_EVENT_CONNECTED:
        esp_mqtt_client_subscribe(event->client, SUBSCRIBE_IN_TOPIC_CONFIG_DEVICE, 0);
        esp_mqtt_client_subscribe(event->client, SUBSCRIBE_IN_TOPIC_CONFIG_ALL_DEVICE, 0);
        break;

    case MQTT_EVENT_DATA: {
      mqtt_received_data_t mqtt_received_data;
      memset(&mqtt_received_data, 0, sizeof(mqtt_received_data));
            
      int t_len = event->topic_len > 63 ? 63 : event->topic_len;
      int d_len = event->data_len > 127 ? 127 : event->data_len;
            
      memcpy(mqtt_received_data.topic, event->topic, t_len);
      mqtt_received_data.topic[t_len] = '\0';
      memcpy(mqtt_received_data.data, event->data, d_len);
      mqtt_received_data.data[d_len] = '\0';

      xQueueSend(mqtt_subscription_queue, &mqtt_received_data, 0);
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
    bool condition_met = (is_internet_available && is_node_root);
      if (condition_met && !mqtt_is_started) {
        ESP_LOGI(TAG, "Internet y Root OK. Iniciando MQTT...");
        esp_mqtt_client_start(client);
        mqtt_is_started = true;
      } else if (!condition_met && mqtt_is_started) {
        ESP_LOGW(TAG, "Internet o Root perdidos. Deteniendo MQTT...");
        esp_mqtt_client_stop(client);
        mqtt_is_started = false;
      }
      vTaskDelay(pdMS_TO_TICKS(2000));
    }
}