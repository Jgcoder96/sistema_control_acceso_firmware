#ifndef CONFIG_H
#define CONFIG_H

#include <stdint.h>

#define WIFI_SSID           "Sin Servicio"
#define WIFI_PWD            "Jesus.040620"

#define MESH_PASSWORD       "MAPLE_MESH_123"
#define MESH_CHANNEL        0                 
#define MESH_MAX_LAYER      6
#define MESH_ID_BYTES       { 0x77, 0x77, 0x77, 0x77, 0x77, 0x77 }


#define TX_INTERVAL_MS      10000              
#define RX_QUEUE_SIZE       10                  
#define STACK_SIZE_TASKS    4096               

#endif