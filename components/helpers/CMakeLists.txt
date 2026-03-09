file(GLOB sources *.c)

idf_component_register(
  SRCS "convert_data_to_json.c"
  INCLUDE_DIRS "include"
  REQUIRES json esp_wifi esp_event wifi_mesh_app
)