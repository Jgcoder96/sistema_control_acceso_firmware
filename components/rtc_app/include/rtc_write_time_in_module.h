#ifndef RTC_WRITE_TIME_IN_MODULE_H
#define RTC_WRITE_TIME_IN_MODULE_H

#include "driver/i2c.h"

esp_err_t rtc_write_time_in_module(struct tm *t);

#endif 