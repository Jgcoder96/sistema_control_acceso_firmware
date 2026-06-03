file(GLOB sources *.c)

idf_component_register(
    SRCS "wifi_mesh_event_handler.c"
         "wifi_mesh_init.c"
         "wifi_mesh_node_logic.c"
         "wifi_mesh_setup.c"
         "wifi_mesh_tasks.c"
         "wifi_mesh_transmission.c"
         "wifi_mesh_monitor_rssi_task.c"
    INCLUDE_DIRS "include"
    REQUIRES esp_wifi esp_event nvs_flash freertos mqtt_app helpers rtc_app node_logic_app nvs_app

)