#ifndef MQTT_PUBLISHER_H
#define MQTT_PUBLISHER_H

/**
 * @file mqtt_publisher.h
 * @brief Interfaz para publicar datos hacia el broker MQTT.
 */

#include "app_types.h"

/**
 * @brief Convierte un paquete interno a JSON y lo publica.
 *
 * Serializa la estructura de datos (ej. lectura de tarjetas) y la envía
 * al tópico MQTT indicado.
 *
 * @par Ejemplo de uso:
 * @code
 * app_packet_t packet;
 * packet.msg_type = MSG_TYPE_CARD_READ;
 * // ... llenar datos ...
 * mqtt_publisher(PUBLISH_IN_TOPIC_CARD_READING, packet);
 * @endcode
 *
 * @param topic Tópico destino en el broker.
 * @param packet Estructura de datos a serializar y enviar.
 */
void mqtt_publisher(const char *topic, app_packet_t packet);

#endif