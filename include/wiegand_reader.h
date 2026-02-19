#ifndef WIEGAND_READER_H
#define WIEGAND_READER_H

#include "esp_err.h"
#include "wiegand_types.h"

esp_err_t wiegand_init(const wiegand_config_t *config);

void wiegand_monitor_task(void *pvParameters);

#endif