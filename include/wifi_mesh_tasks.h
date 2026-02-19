#ifndef MESH_LOGIC_H
#define MESH_LOGIC_H

#include <stdbool.h> 
#include "esp_mesh.h"
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "config.h"

typedef struct {
    uint8_t data[250];       
    uint16_t len;           
    mesh_addr_t source_mac; 
} mesh_packet_t;

typedef enum {
    CHILD_TO_ROOT = 1,        
    ROOT_TO_ALL_CHILDREN = 2, 
    ROOT_TO_CHILD = 3        
} type_t;

typedef struct {
  type_t type;                 
  uint8_t source_mac[6];      
  uint8_t destination_mac[6];  
  int layer;                   
  uint64_t  card_id;          
} app_packet_t;

extern bool is_mesh_connected;      
extern int my_mesh_layer;          
extern uint8_t my_mac[6]; 

extern QueueHandle_t mesh_rx_queue; 
extern QueueHandle_t mesh_tx_queue; 

void mesh_receiver_task(void *arg);
void mesh_transmitter_task(void *arg);
void mesh_processor_task(void *arg);

#endif 