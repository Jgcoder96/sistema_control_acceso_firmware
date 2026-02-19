#ifndef WIEGAND_TYPES_H
#define WIEGAND_TYPES_H

#include <stdint.h>
#include "driver/gpio.h"

typedef enum {
  WIEGAND_FORMAT_UNKNOWN = 0,
  WIEGAND_FORMAT_W26     = 26,
  WIEGAND_FORMAT_W34     = 34
} wiegand_format_t;

typedef struct {
  uint64_t raw_bits;
  uint64_t full_id;           
  uint32_t facility_code;    
  uint32_t card_id;           
  wiegand_format_t format;
  int bit_count;
} wiegand_card_t;

typedef struct {
  gpio_num_t gpio_d0;
  gpio_num_t gpio_d1;
  uint32_t queue_depth;
  int task_priority;
} wiegand_config_t;

#endif