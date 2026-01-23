#include <stdio.h>
#include <string.h>
#include "wiegand_reader.h"
#include "esp_log.h"
#include "esp_timer.h"
#include "freertos/task.h"

static const char *TAG = "WIEGAND_DRV";

static portMUX_TYPE mux = portMUX_INITIALIZER_UNLOCKED;
static volatile uint64_t bit_holder = 0;
static volatile int bit_count = 0;
static volatile uint64_t last_bit_time = 0;
static QueueHandle_t result_queue = NULL;

// ISR para flanco descendente en D0 (Bit 0)
static void IRAM_ATTR isr_handler_d0(void* arg) {
    portENTER_CRITICAL_ISR(&mux);
    bit_holder <<= 1;
    bit_count++;
    last_bit_time = esp_timer_get_time();
    portEXIT_CRITICAL_ISR(&mux);
}

// ISR para flanco descendente en D1 (Bit 1)
static void IRAM_ATTR isr_handler_d1(void* arg) {
    portENTER_CRITICAL_ISR(&mux);
    bit_holder = (bit_holder << 1) | 1;
    bit_count++;
    last_bit_time = esp_timer_get_time();
    portEXIT_CRITICAL_ISR(&mux);
}

static void decode_wiegand(uint64_t raw_bits, int count) {
    wiegand_result_t result = {
        .raw_data = raw_bits,
        .bit_count = count,
        .format = (wiegand_format_t)count,
        .full_id = 0,
        .facility_code = 0,
        .card_id = 0
    };

    if (count == 26) {
        // Wiegand 26: P (1b) + Facility (8b) + CardID (16b) + P (1b)
        // Full ID son los 24 bits centrales
        result.full_id = (raw_bits >> 1) & 0xFFFFFF;
        result.facility_code = (raw_bits >> 17) & 0xFF;
        result.card_id = (raw_bits >> 1) & 0xFFFF;
    } 
    else if (count == 34) {
        // Wiegand 34: P (1b) + Facility (16b) + CardID (16b) + P (1b)
        // Full ID son los 32 bits centrales
        result.full_id = (raw_bits >> 1) & 0xFFFFFFFF;
        result.facility_code = (raw_bits >> 17) & 0xFFFF;
        result.card_id = (raw_bits >> 1) & 0xFFFF;
    } 
    else {
        result.format = WIEGAND_UNKNOWN;
        result.full_id = raw_bits;
    }

    xQueueSend(result_queue, &result, 0);
}

static void wiegand_internal_task(void *pvParameters) {
    while (1) {
        uint64_t now = esp_timer_get_time();
        uint64_t bits_to_process = 0;
        int count_to_process = 0;

        portENTER_CRITICAL(&mux);
        // Si han pasado más de 50ms desde el último pulso, procesar trama
        if (bit_count > 0 && (now - last_bit_time) > 50000) {
            bits_to_process = bit_holder;
            count_to_process = bit_count;
            bit_holder = 0;
            bit_count = 0;
        }
        portEXIT_CRITICAL(&mux);

        if (count_to_process > 0) {
            decode_wiegand(bits_to_process, count_to_process);
        }

        vTaskDelay(pdMS_TO_TICKS(10));
    }
}

esp_err_t wiegand_reader_init(const wiegand_config_t *config, QueueHandle_t *out_queue) {
    if (!config || !out_queue) return ESP_ERR_INVALID_ARG;

    result_queue = xQueueCreate(config->queue_size, sizeof(wiegand_result_t));
    if (!result_queue) return ESP_ERR_NO_MEM;
    *out_queue = result_queue;

    gpio_config_t io_conf = {
        .intr_type = GPIO_INTR_NEGEDGE,
        .mode = GPIO_MODE_INPUT,
        .pin_bit_mask = (1ULL << config->pin_d0) | (1ULL << config->pin_d1),
        .pull_up_en = GPIO_PULLUP_ENABLE,
    };
    gpio_config(&io_conf);

    gpio_install_isr_service(0); 
    gpio_isr_handler_add(config->pin_d0, isr_handler_d0, NULL);
    gpio_isr_handler_add(config->pin_d1, isr_handler_d1, NULL);

    xTaskCreate(wiegand_internal_task, "wiegand_task", 3072, NULL, config->task_priority, NULL);

    return ESP_OK;
}