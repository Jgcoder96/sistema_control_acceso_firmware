#include "driver/i2c.h"    
#include "esp_log.h"       
#include "freertos/FreeRTOS.h" 
#include <stdint.h>

#include "rtc_config.h"
#include "rtc_utils.h"

static const char *TAG = "RTC_GET_TIME_FROM_MODULE";

void rtc_get_time_from_module() {
  uint8_t reg = 0x00;
  uint8_t d[7];
  if (i2c_master_write_read_device(I2C_MASTER_NUM, DS1307_ADDR, &reg, 1, d, 7, pdMS_TO_TICKS(1000)) == ESP_OK) {
    ESP_LOGI(TAG, "RTC: %02d/%02d/20%02d %02d:%02d:%02d",
              bcd_to_decimal(d[4]), bcd_to_decimal(d[5]), bcd_to_decimal(d[6]),
              bcd_to_decimal(d[2]), bcd_to_decimal(d[1]), bcd_to_decimal(d[0]));
  }
}