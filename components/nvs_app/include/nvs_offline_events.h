#ifndef NVS_OFFLINE_EVENTS_H
#define NVS_OFFLINE_EVENTS_H

#include "esp_err.h"
#include "app_types.h"

#define MAX_OFFLINE_LOGS 100 

esp_err_t init_offline_storage(void);

esp_err_t store_event_offline(access_event_t *event);

esp_err_t pop_offline_event(access_event_t *out_event);

uint16_t get_offline_event_count(void);

#endif 