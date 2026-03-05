#ifndef WIEGAND_SETUP_H
#define WIEGAND_SETUP_H

#include "esp_err.h"
#include "wiegand_types.h"

esp_err_t wiegand_setup(const wiegand_config_t *config);

#endif