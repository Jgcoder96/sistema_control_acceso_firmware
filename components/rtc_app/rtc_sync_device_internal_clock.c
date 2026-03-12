#include <time.h>
#include <sys/time.h>
#include "driver/i2c.h"

#include "rtc_config.h"
#include "rtc_utils.h"

// static const char *TAG = "RTC_SYNC_DEVICE_INTERNAL_CLOCK";

void rtc_sync_device_internal_clock() {
  uint8_t reg = 0x00;
  uint8_t d[7];

  if (i2c_master_write_read_device(I2C_MASTER_NUM, DS1307_ADDR, &reg, 1, d, 7, pdMS_TO_TICKS(1000)) == ESP_OK) {  
    struct tm tm;
    tm.tm_sec = bcd_to_decimal(d[0] & 0x7F);
    tm.tm_min = bcd_to_decimal(d[1]);
    tm.tm_hour = bcd_to_decimal(d[2] & 0x3F);
    tm.tm_wday = bcd_to_decimal(d[3]) - 1;
    tm.tm_mday = bcd_to_decimal(d[4]);
    tm.tm_mon = bcd_to_decimal(d[5]) - 1;
    tm.tm_year = bcd_to_decimal(d[6]) + 100;

    time_t t = mktime(&tm);

    struct timeval now = { .tv_sec = t };
    
    settimeofday(&now, NULL);
  }
}