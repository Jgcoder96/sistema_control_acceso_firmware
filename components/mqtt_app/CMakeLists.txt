file(GLOB sources *.c)

set(sources "mqtt_init.c" "mqtt_publisher.c") 

list(APPEND sources "../../src/convert_data_to_json.c")

idf_component_register(
    SRCS "mqtt_init.c"
         "mqtt_publisher.c"
         "mqtt_subscriber.c"
         "mqtt_manager.c"
         "mqtt_subscription_manager.c"
    INCLUDE_DIRS "include"
    REQUIRES mqtt freertos esp_wifi esp_event nvs_flash json helpers
)