file(GLOB sources *.c)

idf_component_register(
    SRCS "nvs_init.c" "nvs_get_version.c" "check_system_version.c"
    INCLUDE_DIRS "include"
    REQUIRES nvs_flash
)