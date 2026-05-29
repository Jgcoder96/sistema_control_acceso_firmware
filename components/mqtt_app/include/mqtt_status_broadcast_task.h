#ifndef MQTT_STATUS_BROADCAST_TASK_H
#define MQTT_STATUS_BROADCAST_TASK_H

void mqtt_status_broadcast_task(void *pvParameters);
void send_mqtt_status_update(void);

#endif