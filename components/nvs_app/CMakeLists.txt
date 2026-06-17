file(GLOB sources *.c)

idf_component_register(
    SRCS "nvs_init.c" 
         "nvs_get_version.c" 
         "nvs_sync_version.c"
         "nvs_save_permissions.c"
         "nvs_offline_events.c"
         "nvs_offline_sync_task.c"
    INCLUDE_DIRS "include"
    REQUIRES nvs_flash freertos helpers wifi_mesh_app mqtt_app
)