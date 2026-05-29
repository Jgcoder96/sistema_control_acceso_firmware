#ifndef RTC_SYNC_H
#define RTC_SYNC_H

#include "esp_err.h"

esp_err_t sync_rtc(uint32_t timestamp_utc);

#endif