#include <stdint.h>

// static const char *TAG = "RTC_UTILS";

int bcd_to_decimal(uint8_t val) { return (int)((val / 16 * 10) + (val % 16)); }

uint8_t decimal_to_bcd(int val) { return (uint8_t)((val / 10 * 16) + (val % 10)); }