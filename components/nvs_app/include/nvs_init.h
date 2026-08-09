#ifndef NVS_INIT_H
#define NVS_INIT_H

/**
 * @file nvs_init.h
 * @brief Inicialización de la memoria NVS y sus tareas.
 */

/// Prioridad de la tarea que sincroniza la versión
#define NVS_SYNC_VERSION_TASK_PRIO 5
/// Stack de la tarea de sincronización de versión
#define NVS_SYNC_VERSION_TASK_SIZE 8192

/// Prioridad de la tarea que despacha eventos offline
#define NVS_OFFLINE_EVENTS_TASK_PRIO 5
/// Stack de la tarea de eventos offline
#define NVS_OFFLINE_EVENTS_TASK_SIZE 4096

/**
 * @brief Inicia todo el entorno NVS (memoria física y tareas).
 *
 * Formatea la partición si es la primera vez, prepara el espacio 
 * circular de logs y lanza las tareas de background.
 *
 * @par Ejemplo de uso:
 * @code
 * void app_main() {
 *     nvs_init(); // Se debe llamar temprano en el boot
 * }
 * @endcode
 */
void nvs_init(void);

/**
 * @brief Solo inicializa el subsistema físico de Flash.
 */
void nvs_storage_init(void);

/**
 * @brief Gatillo manual (semáforo) para forzar un ciclo de sincronización.
 */
void nvs_sync_trigger(void);

#endif