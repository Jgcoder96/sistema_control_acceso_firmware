file(GLOB sources *.c)

idf_component_register(
    SRCS "wiegand_init.c"
         "wiegand_monitor_task.c"
         "wiegand_process_data_task.c"
         "wiegand_setup.c"
    INCLUDE_DIRS "include"
    PRIV_INCLUDE_DIRS "${PROJECT_DIR}/include" 
    REQUIRES freertos driver mqtt_app helpers
)