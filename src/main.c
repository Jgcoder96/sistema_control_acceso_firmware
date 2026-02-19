#include "mqtt_init.h"
#include "wiegand_init.h"
#include "wifi_mesh_init.h"
#include "wifi_mesh_setup.h"

//static const char *TAG = "MAIN_APP";

void app_main(void) {

  wiegand_reader_init();

  wifi_mesh_setup();

  wifi_mesh_init();

  mqtt_init();
}