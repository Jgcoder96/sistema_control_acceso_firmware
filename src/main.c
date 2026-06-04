#include "log_configuration.h"
#include "mqtt_init.h"
#include "nvs_init.h"
#include "rtc_init.h"
#include "wiegand_init.h"
#include "wifi_mesh_event_handler.h"
#include "wifi_mesh_init.h"
#include "wifi_mesh_setup.h"

//static const char *TAG = "MAIN_APP";

void app_main(void) {
  log_configuration();
  
  nvs_init();
  wiegand_init();

  wifi_mesh_setup();
  wifi_mesh_init();

  
  mqtt_init();
  rtc_app_init();

}