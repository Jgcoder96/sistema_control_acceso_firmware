file(GLOB sources *.c)

idf_component_register(
    SRCS "sync_rtc.c"
         "sync_mqtt_status.c"
         "sync_device_trigger.c"
    INCLUDE_DIRS "include"
    REQUIRES rtc_app mqtt_app nvs_app
)