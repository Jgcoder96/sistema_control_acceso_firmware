file(GLOB sources *.c)

idf_component_register(
    SRCS "sync_rtc.c"
         "sync_mqtt_status.c"
    INCLUDE_DIRS "include"
    REQUIRES rtc_app mqtt_app
)