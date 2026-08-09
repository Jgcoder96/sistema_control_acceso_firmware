idf_component_register(
    SRCS "buzzer_app.c"
         "buzzer_init.c"
         "buzzer_task.c"
    INCLUDE_DIRS "include"
    REQUIRES driver freertos
)
