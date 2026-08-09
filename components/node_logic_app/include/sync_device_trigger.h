#ifndef SYNC_DEVICE_TRIGGER_H
#define SYNC_DEVICE_TRIGGER_H

/**
 * @file sync_device_trigger.h
 * @brief Gestión del trigger de sincronización manual.
 */

/**
 * @brief Obliga al nodo a solicitar datos al backend o nodo padre.
 *
 * Libera el semáforo que despierta la tarea encargada de enviar 
 * la petición de actualización de usuarios y permisos.
 *
 * @par Ejemplo de uso:
 * @code
 * // Al recibir orden del botón o de MQTT:
 * sync_device_trigger();
 * @endcode
 */
void sync_device_trigger(void);

#endif