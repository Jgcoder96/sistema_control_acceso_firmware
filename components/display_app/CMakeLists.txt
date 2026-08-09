idf_component_register(
    SRCS "display_app.c"
         "display_init.c"
         "display_task.c"
    INCLUDE_DIRS "include"
    REQUIRES freertos driver wifi_mesh_app
)
