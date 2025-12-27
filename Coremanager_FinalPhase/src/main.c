#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include "scheduler.h"
#include "migrate.h"
#include "energy.h"
#include "ai_scheduler.h"
#include "affinity.h"

void* task_run(void* arg) {
    task_t* t = (task_t*)arg;
    printf("[Task %d] Running on core %d (prio=%d, deadline=%.2f)\n",
           t->id, t->core_assigned, t->priority, t->deadline);
    sleep(1); // simulate work
    t->completed = 1;
    return NULL;
}

int main() {
    printf("🚀 CoreManager — Scheduler Simulation\n");

    print_core_info();  // Show available cores

    scheduler_t sched;
    init_scheduler(&sched, SCHED_PRIORITY);

    task_t tasks[3] = {
        {1, 3, 10.0, 0, 0, 0.0, 0},
        {2, 5, 15.0, 1, 0, 0.0, 0},
        {3, 1, 5.0,  2, 0, 0.0, 0}
    };

    sched.tasks = tasks;
    sched.task_count = 3;

    apply_energy_policy(&sched);
    adjust_priorities_from_ai(&sched);

    int best = select_next_task(&sched);
    printf("Next task to run: Task %d\n", sched.tasks[best].id);
    print_scheduler_state(&sched);

    // Create threads for each task
    for (int i = 0; i < sched.task_count; i++) {
        pthread_create(&sched.tasks[i].thread_id, NULL, task_run, &sched.tasks[i]);
        bind_thread_to_core(sched.tasks[i].thread_id, sched.tasks[i].core_assigned);
    }

    // Simulate a migration
    migrate_task(&sched.tasks[0], 0, 2);

    // Join threads (wait for them to finish)
    for (int i = 0; i < sched.task_count; i++)
        pthread_join(sched.tasks[i].thread_id, NULL);

    printf("✅ Simulation completed.\n");
    return 0;
}
