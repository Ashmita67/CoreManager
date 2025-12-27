#include <stdio.h>
#include "scheduler.h"
#include "energy.h"

void init_scheduler(scheduler_t *sched, scheduler_mode_t mode) {
    sched->mode = mode;
    sched->task_count = 0;
    sched->tasks = NULL;
}

int select_next_task(scheduler_t *sched) {
    if (sched->task_count == 0) {
        printf("No tasks available.\n");
        return -1;
    }

    int best = 0;
    for (int i = 1; i < sched->task_count; i++) {
        if (sched->mode == SCHED_PRIORITY &&
            sched->tasks[i].priority > sched->tasks[best].priority)
            best = i;
        else if (sched->mode == SCHED_DEADLINE &&
                 sched->tasks[i].deadline < sched->tasks[best].deadline)
            best = i;
    }

    return best;
}

void update_scheduler(scheduler_t *sched, int task_id, double runtime) {
    for (int i = 0; i < sched->task_count; i++) {
        if (sched->tasks[i].id == task_id) {
            sched->tasks[i].completed = 1;
            sched->tasks[i].cpu_time = runtime;
            printf("Task %d completed (%.2fs)\n", task_id, runtime);
            break;
        }
    }
}

void print_scheduler_state(const scheduler_t *sched) {
    printf("\n-- Scheduler State --\n");
    for (int i = 0; i < sched->task_count; i++) {
        task_t t = sched->tasks[i];
        printf("Task %d | Prio: %d | Deadline: %.2f | Core: %d | Done: %d\n",
               t.id, t.priority, t.deadline, t.core_assigned, t.completed);
    }
    printf("----------------------\n");
}

task_t *get_highest_priority_task(scheduler_t *sched) {
    int best = select_next_task(sched);
    return &sched->tasks[best];
}
