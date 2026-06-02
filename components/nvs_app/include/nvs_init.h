#ifndef NVS_INIT_H
#define NVS_INIT_H

#define NVS_SYNC_VERSION_TASK_PRIO 5
#define NVS_SYNC_VERSION_TASK_SIZE 3072

void nvs_init(void);
void nvs_storage_init(void);
void nvs_sync_trigger(void);

#endif