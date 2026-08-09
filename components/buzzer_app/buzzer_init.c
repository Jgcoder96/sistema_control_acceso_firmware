/**
 * @file buzzer_init.c
 * @brief Implementación de la inicialización del Buzzer.
 *
 * Configura los temporizadores de hardware PWM (LEDC), reserva memoria
 * para la cola de comandos y lanza la tarea asíncrona de procesamiento.
 */

#include "buzzer_init.h"
#include "buzzer_config.h"
#include "buzzer_task.h"
#include "esp_log.h"

static const char *TAG = "BUZZER_INIT";

void buzzer_app_init(void) {
    // 1. Configurar el Timer del PWM
    ledc_timer_config_t ledc_timer = {
        .speed_mode       = LEDC_MODE,
        .timer_num        = LEDC_TIMER,
        .duty_resolution  = LEDC_DUTY_RES,
        .freq_hz          = 2000,  // Frecuencia inicial por defecto
        .clk_cfg          = LEDC_AUTO_CLK
    };
    esp_err_t err = ledc_timer_config(&ledc_timer);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Error configurando el timer de ledc: %s", esp_err_to_name(err));
        return;
    }

    // 2. Configurar el Canal del PWM
    ledc_channel_config_t ledc_channel = {
        .speed_mode     = LEDC_MODE,
        .channel        = LEDC_CH0_CHANNEL,
        .timer_sel      = LEDC_TIMER,
        .intr_type      = LEDC_INTR_DISABLE,
        .gpio_num       = BUZZER_PIN,
        .duty           = 0, // Empezar apagado
        .hpoint         = 0
    };
    err = ledc_channel_config(&ledc_channel);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Error configurando el canal de ledc: %s", esp_err_to_name(err));
        return;
    }

    // 3. Crear la cola y la tarea
    buzzer_queue = xQueueCreate(5, sizeof(buzzer_cmd_t));
    if (buzzer_queue != NULL) {
        xTaskCreate(buzzer_task, "buzzer_task", BUZZER_TASK_SIZE, NULL, BUZZER_TASK_PRIO, NULL);
    } else {
        ESP_LOGE(TAG, "Error creando la cola del buzzer");
        return;
    }

    ESP_LOGI(TAG, "Buzzer inicializado en el pin %d con tarea asíncrona", BUZZER_PIN);
}
