#ifndef AI_SCHEDULER_H
#define AI_SCHEDULER_H

#include "scheduler.h"

void load_model(const char *path);
void adjust_priorities_from_ai(scheduler_t *sched);

#endif
