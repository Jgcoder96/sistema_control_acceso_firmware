#include <stddef.h>         
#include "freertos/FreeRTOS.h" 
#include "freertos/task.h"     

#include "rtc_init.h"
#include "rtc_management_task.h"

// static const char *TAG = "RTC_INIT";

void rtc_app_init(void) {
  xTaskCreate(rtc_management_task, "rtc_management_task", RTC_MANAGEMENT_TASK_SIZE, NULL, RTC_MANAGEMENT_TASK_PRIO, NULL);
}