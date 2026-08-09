#ifndef MQTT_SUBSCRIBER_H
#define MQTT_SUBSCRIBER_H

/**
 * @file mqtt_subscriber.h
 * @brief Recepción y enrutamiento de mensajes MQTT.
 */

/**
 * @brief Escucha mensajes MQTT entrantes y los despacha según su tópico.
 *
 * Toma los payloads de la cola, los parsea como cJSON y llama a la función
 * manejadora correspondiente (sincronización o trigger).
 *
 * @par Ejemplo de enrutamiento:
 * @code
 * if (strcmp(topic, "device/sync/response") == 0) {
 *    subscription_manager_sync_device(json_root);
 * }
 * @endcode
 */
void mqtt_subscription_handler_task(void *pvParameters);

#endif