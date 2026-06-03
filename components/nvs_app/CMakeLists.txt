file(GLOB sources *.c)

idf_component_register(
    SRCS "nvs_init.c" 
         "nvs_get_version.c" 
         "nvs_sync_version.c"
         "check_system_version.c"
         "nvs_save_permissions.c"
    INCLUDE_DIRS "include"
    REQUIRES nvs_flash freertos helpers wifi_mesh_app mqtt_app
)