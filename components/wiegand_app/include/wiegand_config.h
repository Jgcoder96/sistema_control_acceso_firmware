#ifndef WIEGAND_CONFIG_H
#define WIEGAND_CONFIG_H

#include "wiegand_types.h"

#define WIEGAND_GPIO_D0 GPIO_NUM_12
#define WIEGAND_GPIO_D1 GPIO_NUM_11

static const wiegand_config_t WIEGAND_CONFIG_DEFAULT = {
  .gpio_d0 = WIEGAND_GPIO_D0,
  .gpio_d1 = WIEGAND_GPIO_D1
};

#endif