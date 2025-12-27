#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <pthread.h>

typedef enum {
    SCHED_PRIORITY,
    SCHED_DEADLINE,
    SCHED_AI
} scheduler_mode_t;

typedef struct {
    int id;
    int priority;
    double deadline;
    int core_assigned;
    pthread_t thread_id;
    double cpu_time;
    int completed;
} task_t;

typedef struct {
    scheduler_mode_t mode;
    int task_count;
    task_t *tasks;
} scheduler_t;

void init_scheduler(scheduler_t *sched, scheduler_mode_t mode);
int select_next_task(scheduler_t *sched);
void update_scheduler(scheduler_t *sched, int task_id, double runtime);
void print_scheduler_state(const scheduler_t *sched);
task_t *get_highest_priority_task(scheduler_t *sched);

#endif
