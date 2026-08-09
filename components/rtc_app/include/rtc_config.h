#ifndef RTC_CONFIG_H
#define RTC_CONFIG_H

#include "esp_err.h"
#include "time.h"

/**
 * @file rtc_config.h
 * @brief Configuración global del módulo RTC (I2C y NTP).
 */

// --- Configuración del bus I2C ---
#define I2C_MASTER_SDA_IO   41      
#define I2C_MASTER_SCL_IO   42      
#define I2C_MASTER_NUM      I2C_NUM_0
#define I2C_MASTER_FREQ_HZ  100000
#define DS1307_ADDR         0x68

// --- Servidores NTP (Time Servers) ---
#define NTP_SERVER          "pool.ntp.org"
#define NTP_SERVER_BACKUP   "time.google.com"

/// Zona horaria por defecto (Venezuela = UTC-4)
#define DEFAULT_TIMEZONE    "VET4"  

/// Frecuencia de resincronización automática con NTP
#define SYNC_INTERVAL_MS    (24 * 60 * 60 * 1000) 

/// Reintentos máximos si el servidor NTP falla
#define NTP_MAX_RETRY       20      
#define NTP_RETRY_DELAY_MS  2000  

#endif 