#ifndef RTC_UTILS_H
#define RTC_UTILS_H

#include <stdint.h>

int bcd_to_decimal(uint8_t val);
uint8_t decimal_to_bcd(int val);

#endif 