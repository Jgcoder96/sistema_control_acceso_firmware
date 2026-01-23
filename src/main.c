#include <stdio.h>
#include <inttypes.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "wiegand_reader.h"

static const char *TAG = "MAIN_APP";

void app_main(void) {
    QueueHandle_t rfid_queue = NULL;

    wiegand_config_t config = {
        .pin_d0 = GPIO_NUM_4,
        .pin_d1 = GPIO_NUM_5,
        .queue_size = 10,
        .task_priority = 10
    };

    if (wiegand_reader_init(&config, &rfid_queue) == ESP_OK) {
        ESP_LOGI(TAG, "Lector Wiegand Listo.");
        
        wiegand_result_t data;
        while (1) {
            if (xQueueReceive(rfid_queue, &data, portMAX_DELAY)) {
                
                ESP_LOGI(TAG, "============================================");
                // Imprime el ID largo con ceros iniciales (Total 10 dígitos)
                ESP_LOGI(TAG, "TARJETA (Decimal): %010llu", data.full_id);
                
                // Imprime el formato impreso tipo FC,CardID
                ESP_LOGI(TAG, "FORMATO IMPRESO:  %03lu,%05lu", 
                         data.facility_code, data.card_id);
                
                ESP_LOGI(TAG, "DETALLES TÉCNICOS:");
                ESP_LOGI(TAG, "  - Formato: W%d", (int)data.format);
                ESP_LOGI(TAG, "  - RAW Hex: 0x%llX", data.raw_data);
                ESP_LOGI(TAG, "============================================\n");
            }
        }
    } else {
        ESP_LOGE(TAG, "Error inicializando el lector");
    }
}