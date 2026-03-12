#include "driver/i2c.h"

#include "rtc_config.h"
#include "rtc_utils.h"

// static const char *TAG = "RTC_WRITE_TIME_IN_MODULE";

esp_err_t rtc_write_time_in_module(struct tm *t) {
    uint8_t data[8];

    data[0] = 0x00; 
    data[1] = decimal_to_bcd(t->tm_sec);
    data[2] = decimal_to_bcd(t->tm_min);
    data[3] = decimal_to_bcd(t->tm_hour);
    data[4] = decimal_to_bcd(t->tm_wday + 1); 
    data[5] = decimal_to_bcd(t->tm_mday);
    data[6] = decimal_to_bcd(t->tm_mon + 1);
    data[7] = decimal_to_bcd(t->tm_year % 100);
    
    return i2c_master_write_to_device(I2C_MASTER_NUM, DS1307_ADDR, data, 8, pdMS_TO_TICKS(1000));
}