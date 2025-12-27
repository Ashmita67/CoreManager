#include <stdio.h>
#include "migrate.h"
#include "affinity.h"

void migrate_task(task_t *task, int from_core, int to_core) {
    printf("Migrating Task %d: Core %d → Core %d\n",
           task->id, from_core, to_core);
    bind_thread_to_core(task->thread_id, to_core);
    task->core_assigned = to_core;
}
