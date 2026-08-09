/**
 * @file display_task.c
 * @brief Implementación de la tarea asíncrona de la Pantalla OLED.
 *
 * Contiene el bucle principal, renderizado del búfer y el driver I2C por software.
 */

#include "display_task.h"
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "rom/ets_sys.h"
#include "wifi_mesh_info.h"

static const char *TAG = "DISPLAY_APP";

#include "display_config.h"

// --- I2C POR SOFTWARE (BIT-BANGING) ---
#define I2C_DELAY() ets_delay_us(5)

/**
 * @brief Inicializa los pines GPIO para emular I2C por software (Bit-Banging).
 *
 * Configura los pines de SDA y SCL en modo Open-Drain y los establece en alto (estado inactivo).
 */
static void sw_i2c_init(void) {
    gpio_reset_pin(SDA_PIN);
    gpio_reset_pin(SCL_PIN);
    gpio_set_direction(SDA_PIN, GPIO_MODE_INPUT_OUTPUT_OD);
    gpio_set_direction(SCL_PIN, GPIO_MODE_INPUT_OUTPUT_OD);
    gpio_set_level(SDA_PIN, 1);
    gpio_set_level(SCL_PIN, 1);
    I2C_DELAY();
}

/**
 * @brief Genera la condición de INICIO (Start) en el bus I2C.
 *
 * Una transición de ALTO a BAJO en la línea SDA mientras SCL está en ALTO.
 */
static void sw_i2c_start(void) {
    gpio_set_level(SDA_PIN, 1);
    gpio_set_level(SCL_PIN, 1);
    I2C_DELAY();
    gpio_set_level(SDA_PIN, 0);
    I2C_DELAY();
    gpio_set_level(SCL_PIN, 0);
    I2C_DELAY();
}

/**
 * @brief Genera la condición de PARADA (Stop) en el bus I2C.
 *
 * Una transición de BAJO a ALTO en la línea SDA mientras SCL está en ALTO.
 */
static void sw_i2c_stop(void) {
    gpio_set_level(SDA_PIN, 0);
    I2C_DELAY();
    gpio_set_level(SCL_PIN, 1);
    I2C_DELAY();
    gpio_set_level(SDA_PIN, 1);
    I2C_DELAY();
}

/**
 * @brief Transmite un solo byte a través del bus I2C simulado.
 *
 * Envía bit a bit (MSB primero) con los retardos correspondientes para estabilizar
 * la señal de reloj. Ignora intencionalmente el bit ACK del esclavo (pantalla).
 *
 * @param data El byte (8 bits) a enviar.
 */
static void sw_i2c_write_byte(uint8_t data) {
    for (int i = 7; i >= 0; i--) {
        gpio_set_level(SDA_PIN, (data >> i) & 1);
        I2C_DELAY();
        gpio_set_level(SCL_PIN, 1);
        I2C_DELAY();
        gpio_set_level(SCL_PIN, 0);
        I2C_DELAY();
    }
    // ACK bit (ignorar)
    gpio_set_level(SDA_PIN, 1);
    I2C_DELAY();
    gpio_set_level(SCL_PIN, 1);
    I2C_DELAY();
    gpio_set_level(SCL_PIN, 0);
    I2C_DELAY();
}

// Buffer de pantalla (128x64 píxeles)
static uint8_t screen_buffer[1024];

// Fuente 5x7 Completa (0-9, :, /, -, A-Z, Espacio, .)
static const uint8_t font_full[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, // [0] Espacio
    0x3E, 0x51, 0x49, 0x45, 0x3E, // [1] 0
    0x00, 0x42, 0x7F, 0x40, 0x00, // [2] 1
    0x42, 0x61, 0x51, 0x49, 0x46, // [3] 2
    0x21, 0x41, 0x45, 0x4B, 0x31, // [4] 3
    0x18, 0x14, 0x12, 0x7F, 0x10, // [5] 4
    0x27, 0x45, 0x45, 0x45, 0x39, // [6] 5
    0x3C, 0x4A, 0x49, 0x49, 0x30, // [7] 6
    0x01, 0x71, 0x09, 0x05, 0x03, // [8] 7
    0x36, 0x49, 0x49, 0x49, 0x36, // [9] 8
    0x06, 0x49, 0x49, 0x29, 0x1E, // [10] 9
    0x00, 0x36, 0x36, 0x00, 0x00, // [11] :
    0x20, 0x10, 0x08, 0x04, 0x02, // [12] /
    0x08, 0x08, 0x08, 0x08, 0x08, // [13] -
    0x7C, 0x12, 0x11, 0x12, 0x7C, // [14] A
    0x7F, 0x49, 0x49, 0x49, 0x36, // [15] B
    0x3E, 0x41, 0x41, 0x41, 0x22, // [16] C
    0x7F, 0x41, 0x41, 0x22, 0x1C, // [17] D
    0x7F, 0x49, 0x49, 0x49, 0x41, // [18] E
    0x7F, 0x09, 0x09, 0x09, 0x01, // [19] F
    0x3E, 0x41, 0x49, 0x49, 0x7A, // [20] G
    0x7F, 0x08, 0x08, 0x08, 0x7F, // [21] H
    0x00, 0x41, 0x7F, 0x41, 0x00, // [22] I
    0x20, 0x40, 0x41, 0x3F, 0x01, // [23] J
    0x7F, 0x08, 0x14, 0x22, 0x41, // [24] K
    0x7F, 0x40, 0x40, 0x40, 0x40, // [25] L
    0x7F, 0x02, 0x0C, 0x02, 0x7F, // [26] M
    0x7F, 0x04, 0x08, 0x10, 0x7F, // [27] N
    0x3E, 0x41, 0x41, 0x41, 0x3E, // [28] O
    0x7F, 0x09, 0x09, 0x09, 0x06, // [29] P
    0x3E, 0x41, 0x51, 0x21, 0x5E, // [30] Q
    0x7F, 0x09, 0x19, 0x29, 0x46, // [31] R
    0x46, 0x49, 0x49, 0x49, 0x31, // [32] S
    0x01, 0x01, 0x7F, 0x01, 0x01, // [33] T
    0x3F, 0x40, 0x40, 0x40, 0x3F, // [34] U
    0x1F, 0x20, 0x40, 0x20, 0x1F, // [35] V
    0x3F, 0x40, 0x38, 0x40, 0x3F, // [36] W
    0x63, 0x14, 0x08, 0x14, 0x63, // [37] X
    0x07, 0x08, 0x70, 0x08, 0x07, // [38] Y
    0x61, 0x51, 0x49, 0x45, 0x43, // [39] Z
    0x00, 0x60, 0x60, 0x00, 0x00  // [40] .
};

// --- COMUNICACIONES OLED ---
/**
 * @brief Envía un comando a la pantalla OLED mediante I2C.
 *
 * Envía la dirección I2C, un byte de control `0x00` (indicando que es comando) y el dato.
 *
 * @param cmd El comando (byte) que se interpretará internamente en la pantalla.
 */
static void oled_cmd(uint8_t cmd) {
    sw_i2c_start();
    sw_i2c_write_byte(OLED_ADDR << 1);
    sw_i2c_write_byte(0x00);
    sw_i2c_write_byte(cmd);
    sw_i2c_stop();
}

// --- VARIABLES DE ESTADO PARA POPUP ---
int current_popup = 0; // 0 = normal, 1 = concedido, 2 = denegado
uint32_t popup_timeout_tick = 0;

/**
 * @brief Actualiza la pantalla OLED completa volcando el búfer de memoria.
 *
 * Transfiere los 1024 bytes (8 páginas * 128 columnas) hacia la memoria GDDRAM 
 * del controlador de la pantalla OLED utilizando I2C.
 */
static void oled_update(void) {
    for (uint8_t i = 0; i < 8; i++) {
        oled_cmd(0xB0 + i);
        oled_cmd(0x00); // Columna 0 (antes era 0x02)
        oled_cmd(0x10);

        sw_i2c_start();
        sw_i2c_write_byte(OLED_ADDR << 1);
        sw_i2c_write_byte(0x40);
        for (int j = 0; j < 128; j++) {
            sw_i2c_write_byte(screen_buffer[i * 128 + j]);
        }
        sw_i2c_stop();
    }
}

// --- DIBUJO EN BUFFER ---
/**
 * @brief Limpia el búfer de pantalla (llena con ceros).
 */
static void buf_clear(void) {
    memset(screen_buffer, 0, 1024);
}

/**
 * @brief Dibuja texto ASCII en el búfer interno.
 *
 * Busca en la fuente `font_full` y copia los 5 bytes de cada carácter al búfer de video.
 *
 * @param page Fila (página GDDRAM) de la 0 a la 7.
 * @param col Columna (pixel GDDRAM) de la 0 a la 127.
 * @param txt Cadena de texto a escribir.
 */
static void buf_draw_text(uint8_t page, uint8_t col, const char *txt) {
    if (page > 7) return;
    for (int i = 0; i < strlen(txt); i++) {
        uint16_t idx = 0;
        char c = txt[i];
        if (c >= '0' && c <= '9') idx = (c - '0' + 1) * 5;
        else if (c == ':') idx = 11 * 5;
        else if (c == '/') idx = 12 * 5;
        else if (c == '-') idx = 13 * 5;
        else if (c >= 'A' && c <= 'Z') idx = (c - 'A' + 14) * 5;
        else if (c >= 'a' && c <= 'z') idx = (c - 'a' + 14) * 5;
        else if (c == '.') idx = 40 * 5;
        else idx = 0;

        for (int j = 0; j < 5; j++) {
            int pos = page * 128 + col + (i * 6) + j;
            if (pos < 1024) screen_buffer[pos] = font_full[idx + j];
        }
    }
}

/**
 * @brief Dibuja el icono de la intensidad de la señal (barras tipo celular) en el búfer.
 *
 * Ocupa un área predeterminada en la esquina superior derecha (columna 115).
 *
 * @param bars Número de barras a encender (de 0 a 4).
 */
static void buf_draw_signal(uint8_t bars) {
    uint8_t col = 115;
    if (bars >= 1) { screen_buffer[col] = 0x60; screen_buffer[col+1] = 0x60; }
    if (bars >= 2) { screen_buffer[col+3] = 0x78; screen_buffer[col+4] = 0x78; }
    if (bars >= 3) { screen_buffer[col+6] = 0x7E; screen_buffer[col+7] = 0x7E; }
    if (bars >= 4) { screen_buffer[col+9] = 0x7F; screen_buffer[col+10] = 0x7F; }
}

/**
 * @brief Rutina de inicialización hardware de la pantalla OLED (SSD1306).
 *
 * Envía la secuencia estandarizada de comandos mágicos para activar el panel OLED
 * y configurar el mapeo de memoria y voltaje.
 */
static void oled_init(void) {
    vTaskDelay(pdMS_TO_TICKS(200));
    uint8_t init_cmds[] = {
        0xAE, 0xD5, 0x80, 0xA8, 0x3F, 0xD3, 0x00, 0x40,
        0x8D, 0x14, 0x20, 0x02, 0xA1, 0xC8, 0xDA, 0x12,
        0x81, 0xCF, 0xD9, 0xF1, 0xDB, 0x40, 0xA4, 0xA6, 0xAF
    };
    for (int i = 0; i < sizeof(init_cmds); i++) oled_cmd(init_cmds[i]);
}

/**
 * @brief Convierte un valor de RSSI en un número de barras (0 a 4).
 *
 * @param rssi Potencia de la señal Wi-Fi Mesh en dBm (Ej. -65).
 * @return Número discreto de barras para renderizar el icono de señal.
 */
static uint8_t rssi_to_bars(int rssi) {
    if (rssi == 0) return 0;
    if (rssi > -60) return 4;
    if (rssi > -75) return 3;
    if (rssi > -85) return 2;
    return 1;
}

/**
 * @brief Tarea asíncrona principal para el control de la pantalla.
 *
 * Esta tarea maneja:
 * - La inicialización del bus I2C y hardware de la pantalla.
 * - La captura del tiempo de sistema y el cálculo de estado del Mesh (red).
 * - El renderizado periódico en pantalla (actualización cada 1 segundo).
 * - La visualización rápida de pop-ups transitorios a petición externa.
 *
 * @param pvParameters Parámetros por defecto de FreeRTOS.
 */
void display_task(void *pvParameters) {
    // Esperar a que el sistema se estabilice
    vTaskDelay(pdMS_TO_TICKS(2000));

    // Inicializar I2C por software y OLED
    sw_i2c_init();
    oled_init();
    ESP_LOGI(TAG, "Pantalla OLED inicializada (SW I2C)");

    char str_mac[32], str_mesh[32], str_date[32], str_time[32];

    while (1) {
        time_t now;
        time(&now);
        struct tm t;
        localtime_r(&now, &t);

        snprintf(str_date, sizeof(str_date), "%02d/%02d/%04d", t.tm_mday, t.tm_mon + 1, t.tm_year + 1900);
        snprintf(str_time, sizeof(str_time), "%02d:%02d:%02d", t.tm_hour, t.tm_min, t.tm_sec);

        snprintf(str_mesh, sizeof(str_mesh), "ID:%02X:%02X:%02X:%02X:%02X:%02X",
                 node_mesh_info.mesh_id.addr[0], node_mesh_info.mesh_id.addr[1], node_mesh_info.mesh_id.addr[2],
                 node_mesh_info.mesh_id.addr[3], node_mesh_info.mesh_id.addr[4], node_mesh_info.mesh_id.addr[5]);

        snprintf(str_mac, sizeof(str_mac), "MAC:%02X:%02X:%02X:%02X:%02X:%02X",
                 node_mesh_info.mac[0], node_mesh_info.mac[1], node_mesh_info.mac[2],
                 node_mesh_info.mac[3], node_mesh_info.mac[4], node_mesh_info.mac[5]);

        uint8_t bars = rssi_to_bars(node_mesh_info.rssi);

        buf_clear();

        uint32_t current_tick = xTaskGetTickCount();
        if (current_popup != 0) {
            if (current_tick < popup_timeout_tick) {
                // Dibujar popup en el centro de la pantalla
                if (current_popup == 1) {
                    buf_draw_text(3, 16, "ACCESO CONCEDIDO");
                } else {
                    buf_draw_text(3, 20, "ACCESO DENEGADO");
                }
                oled_update();
                vTaskDelay(pdMS_TO_TICKS(100)); // Actualización más rápida durante el popup
                continue;
            } else {
                // El tiempo expiró, limpiar estado y volver a la pantalla normal
                current_popup = 0;
            }
        }

        // --- PANTALLA NORMAL ---
        // --- FILA 0: Rol y Señal ---
        if (node_mesh_info.is_root) {
            buf_draw_text(0, 0, "NODO ROOT");
        } else {
            buf_draw_text(0, 0, "NODO INT.");
        }

        // Mostrar siempre el nivel de señal si la malla está conectada
        if (node_mesh_info.is_mesh_connected) {
            buf_draw_signal(bars);
        }

        // --- FILA 2 y 3: Información de Red ---
        buf_draw_text(2, 0, str_mesh);
        buf_draw_text(3, 0, str_mac);

        // --- FILA 6 y 7: Fecha y Hora ---
        buf_draw_text(6, 32, str_date);
        buf_draw_text(7, 40, str_time);

        oled_update();

        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}


