#ifndef MIGRATE_H
#define MIGRATE_H

#include "scheduler.h"

void migrate_task(task_t *task, int from_core, int to_core);

#endif
