#include "log_configuration.h"
#include "mqtt_init.h"
#include "nvs_init.h"
#include "rtc_init.h"
#include "wiegand_init.h"
#include "wifi_mesh_event_handler.h"
#include "wifi_mesh_init.h"
#include "wifi_mesh_setup.h"
#include "rgb_led.h"
#include "esp_log.h"

#include "driver/gpio.h"

static const char *TAG = "MAIN_APP";

void app_main(void) {
  log_configuration();

  gpio_reset_pin(GPIO_NUM_33);
  gpio_set_direction(GPIO_NUM_33, GPIO_MODE_OUTPUT);
  gpio_set_level(GPIO_NUM_33, 1); 

  rgb_led_init();
    
  nvs_init();
  wiegand_init();

  wifi_mesh_setup();
  wifi_mesh_init();

  mqtt_init();
  rtc_app_init();

}