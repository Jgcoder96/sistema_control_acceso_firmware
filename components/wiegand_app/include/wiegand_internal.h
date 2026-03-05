#ifndef WIEGAND_INTERNAL_H
#define WIEGAND_INTERNAL_H

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_timer.h"

typedef struct {
  uint64_t bit_buffer;
  volatile int bit_count;
  volatile uint64_t last_bit_time_us;
  portMUX_TYPE mux;
} wiegand_context_t;

extern wiegand_context_t *g_wiegand_ctx;

#endif