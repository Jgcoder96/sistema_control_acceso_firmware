#ifndef APP_TYPES_H
#define APP_TYPES_H

#include "esp_mesh.h"

typedef struct {
  uint8_t data[250];       
  uint16_t len;           
  mesh_addr_t source_mac; 
} mesh_packet_t;

typedef enum {
  CHILD_TO_ROOT = 1,        
  ROOT_TO_ALL_CHILDREN = 2, 
  ROOT_TO_CHILD = 3        
} type_mesh_t;

typedef enum {
  MSG_TYPE_CARD,     
  MSG_TYPE_CONFIG_DEVICE,
  MSG_TYPE_CONFIG_ALL_DEVICES,
  MSG_TYPE_RTC_SYNC,
  MSG_TYPE_MQTT_STATUS,
  MSG_TYPE_INITIAL_SYNC
} type_msg_t;

typedef struct {
  char card_id[21];     
  uint32_t timestamp;   
} access_event_t;

typedef enum {
  SYNC_STATE_NONE,     
  SYNC_STATE_CHECK_VER    
} sync_state_t;

typedef struct {
  sync_state_t state;    
  uint32_t version;    
} initial_sync_event_t;

typedef struct {
  bool mqtt_status;
} mqtt_status_event_t;

typedef struct {
  type_mesh_t type;
  type_msg_t msg_type;
         
  uint8_t source_mac[6];      
  uint8_t destination_mac[6];                   
    
  union {
    access_event_t access_event; 
    mqtt_status_event_t mqtt_status_event;
    initial_sync_event_t initial_sync_event;

    char accion[16];   
    uint32_t timestamp;      
  } payload;                   
} app_packet_t;

#endif