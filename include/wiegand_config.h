#ifndef WIEGAND_CONFIG_H
#define WIEGAND_CONFIG_H

#include "wiegand_types.h"

#define WIEGAND_GPIO_D0         GPIO_NUM_4
#define WIEGAND_GPIO_D1         GPIO_NUM_5

#define WIEGAND_QUEUE_LENGTH    10
#define WIEGAND_DRIVER_PRIO     12  
#define CARD_PROCESSOR_PRIO     10 

static const wiegand_config_t WIEGAND_CONFIG_DEFAULT = {
  .gpio_d0 = WIEGAND_GPIO_D0,
  .gpio_d1 = WIEGAND_GPIO_D1,
  .queue_depth = WIEGAND_QUEUE_LENGTH,
  .task_priority = WIEGAND_DRIVER_PRIO
};

#endif