file(GLOB sources *.c)

idf_component_register(
    SRCS "rtc_get_time_from_module.c"
         "rtc_i2c_config.c"
         "rtc_init.c"
         "rtc_management_task.c"
         "rtc_set_time_from_ntp_server.c"
         "rtc_sync_device_internal_clock.c"
         "rtc_utils.c"
         "rtc_write_time_in_module.c"
    INCLUDE_DIRS "include"
    REQUIRES driver lwip esp_netif log freertos
)