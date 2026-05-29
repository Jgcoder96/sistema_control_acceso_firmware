#include "mqtt_init.h"
#include "rtc_init.h"
#include "wiegand_init.h"
#include "wifi_mesh_init.h"
#include "wifi_mesh_setup.h"
#include "nvs_init.h"
#include "check_system_version.h"
#include "wifi_mesh_event_handler.h"
#include "log_configuration.h"

//static const char *TAG = "MAIN_APP";

void app_main(void) {
  log_configuration();
  
  nvs_init();
  wiegand_init();

  wifi_mesh_setup();
  wifi_mesh_init();

  
  mqtt_init();
  rtc_app_init();
  

  //check_system_version();

}