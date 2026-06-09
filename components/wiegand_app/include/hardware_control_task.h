#ifndef HARDWARE_CONTROL_TASK_H
#define HARDWARE_CONTROL_TASK_H

#define PIN_RELE      37
#define PIN_LED_L2    38
#define PIN_LED_L1    39

typedef struct {
  bool permitted;
} hardware_cmd_t;

void hardware_control_task(void *pvParameters);

#endif