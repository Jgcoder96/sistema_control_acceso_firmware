#ifndef MQTT_CONFIG_H
#define MQTT_CONFIG_H

/**
 * @file mqtt_config.h
 * @brief Constantes globales de configuración para MQTT.
 */

/// URL de conexión al broker MQTT central
#define MQTT_BROKER_URL "mqtt://192.168.18.221:1883"

/// Tópico usado para reportar tarjetas RFID/Wiegand leídas
#define PUBLISH_IN_TOPIC_CARD_READING "mesh/config"

/// Tópico en el que se espera recibir el JSON con los permisos de usuarios
#define SUBSCRIBE_IN_TOPIC_SYNC_RESPONSE "device/sync/response"

/// Tópico que obliga al dispositivo a pedir sus permisos al backend
#define SUBSCRIBE_IN_TOPIC_SYNC_TRIGGER "device/sync/trigger"

#endif