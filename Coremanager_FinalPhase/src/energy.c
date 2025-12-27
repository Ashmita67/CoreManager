#include <stdio.h>
#include "energy.h"

void apply_energy_policy(scheduler_t *sched) {
    if (sched->task_count == 0) return;

    double avg = 0;
    for (int i = 0; i < sched->task_count; i++)
        avg += sched->tasks[i].priority * 10.0;

    avg /= sched->task_count;

    if (avg < 30)
        printf("[Energy] Low usage (%.2f%%): Consolidating tasks\n", avg);
    else if (avg > 70)
        printf("[Energy] High usage (%.2f%%): Activating more cores\n", avg);
    else
        printf("[Energy] Balanced (%.2f%%)\n", avg);
}
