#ifndef NVS_GET_VERSION_H
#define NVS_GET_VERSION_H

#include "esp_err.h"

esp_err_t nvs_get_version(char* version_out, size_t max_len);

#endif