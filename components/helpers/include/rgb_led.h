#ifndef RGB_LED_H
#define RGB_LED_H

#include <stdint.h>
#include "esp_err.h"

esp_err_t rgb_led_init(void); 

esp_err_t rgb_led_set_color(uint8_t r, uint8_t g, uint8_t b);
esp_err_t rgb_led_clear(void);

#endif