#include "buzzer_app.h"
#include "driver/ledc.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

static const char *TAG = "BUZZER_APP";

#define BUZZER_PIN          (40)
#define LEDC_TIMER          LEDC_TIMER_0
#define LEDC_MODE           LEDC_LOW_SPEED_MODE
#define LEDC_CH0_CHANNEL    LEDC_CHANNEL_0
#define LEDC_DUTY_RES       LEDC_TIMER_10_BIT // Resolución de 10 bits (0-1023)
#define LEDC_DUTY           (512)            // 50% de ciclo de trabajo

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

    ESP_LOGI(TAG, "Buzzer inicializado en el pin %d", BUZZER_PIN);
}

void buzzer_app_beep(uint32_t freq, uint32_t duration_ms) {
    if (freq > 0) {
        // Configurar la nueva frecuencia
        ledc_set_freq(LEDC_MODE, LEDC_TIMER, freq);
        // Establecer el duty cycle al 50% para generar la onda
        ledc_set_duty(LEDC_MODE, LEDC_CH0_CHANNEL, LEDC_DUTY);
        ledc_update_duty(LEDC_MODE, LEDC_CH0_CHANNEL);
    } else {
        // Si la frecuencia es 0, silenciar
        ledc_set_duty(LEDC_MODE, LEDC_CH0_CHANNEL, 0);
        ledc_update_duty(LEDC_MODE, LEDC_CH0_CHANNEL);
    }

    // Esperar el tiempo indicado
    vTaskDelay(pdMS_TO_TICKS(duration_ms));

    // Apagar después de que pase el tiempo
    ledc_set_duty(LEDC_MODE, LEDC_CH0_CHANNEL, 0);
    ledc_update_duty(LEDC_MODE, LEDC_CH0_CHANNEL);
}
