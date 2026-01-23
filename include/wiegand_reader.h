#ifndef WIEGAND_READER_H
#define WIEGAND_READER_H

#include "esp_err.h"
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    WIEGAND_UNKNOWN = 0,
    WIEGAND_26 = 26,
    WIEGAND_34 = 34
} wiegand_format_t;

typedef struct {
    uint64_t raw_data;      // Trama completa en hexadecimal
    uint64_t full_id;       // El número largo (ej: 10682912)
    uint32_t facility_code; // Código de instalación (ej: 163)
    uint32_t card_id;       // ID de tarjeta corto (ej: 544)
    wiegand_format_t format;
    int bit_count;
} wiegand_result_t;

typedef struct {
    gpio_num_t pin_d0;
    gpio_num_t pin_d1;
    uint32_t queue_size;
    int task_priority;
} wiegand_config_t;

/**
 * @brief Inicializa el componente Wiegand.
 */
esp_err_t wiegand_reader_init(const wiegand_config_t *config, QueueHandle_t *out_queue);

#ifdef __cplusplus
}
#endif

#endif