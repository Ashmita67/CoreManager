#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TASKS 200
#define MAX_CORES 16
#define TIME_STEP_MS 100

typedef enum { READY = 0, RUNNING = 1, FINISHED = 2 } TaskState;

typedef struct {
    int id;
    int priority;
    int remaining_ms;
    TaskState state;
} Task;

typedef struct {
    int id;
    Task *assigned_task;
} Core;

static Task tasks[MAX_TASKS];
static int task_count = 0;
static Core cores[MAX_CORES];
static int num_cores = 2;

void init_cores(int n) {
    if (n < 1) n = 1;
    if (n > MAX_CORES) n = MAX_CORES;
    num_cores = n;
    for (int i = 0; i < num_cores; ++i) {
        cores[i].id = i;
        cores[i].assigned_task = NULL;
    }
}

void create_task_cli() {
    if (task_count >= MAX_TASKS) { printf("Max tasks reached.\n"); return; }
    int prio, time_ms;
    printf("Enter priority (1-10): ");
    if (scanf("%d", &prio) != 1) { while(getchar()!='\n'); printf("Invalid\n"); return; }
    if (prio < 1) prio = 1; if (prio > 10) prio = 10;
    printf("Enter required time in ms (e.g., 500): ");
    if (scanf("%d", &time_ms) != 1) { while(getchar()!='\n'); printf("Invalid\n"); return; }

    Task t;
    t.id = task_count + 1;
    t.priority = prio;
    t.remaining_ms = time_ms;
    t.state = READY;
    tasks[task_count++] = t;

    printf("Created Task %d (prio=%d, time=%d ms)\n", t.id, t.priority, t.remaining_ms);
}

void list_tasks() {
    if (task_count == 0) { printf("No tasks created.\n"); return; }
    printf("\nTasks:\nID  Prio  Rem(ms)  State\n");
    for (int i = 0; i < task_count; ++i) {
        const char *s = (tasks[i].state==READY) ? "READY" :
                        (tasks[i].state==RUNNING) ? "RUNNING" : "FINISHED";
        printf("%-3d %-5d %-8d %s\n", tasks[i].id, tasks[i].priority, tasks[i].remaining_ms, s);
    }
}

void show_cores() {
    printf("\nCores:\n");
    for (int i = 0; i < num_cores; ++i) {
        if (cores[i].assigned_task) {
            Task *t = cores[i].assigned_task;
            printf("Core %d -> Task %d (prio=%d, remaining=%d ms, state=%s)\n",
                   i, t->id, t->priority, t->remaining_ms,
                   (t->state==RUNNING) ? "RUNNING" : "FINISHED");
        } else {
            printf("Core %d -> IDLE\n", i);
        }
    }
}

void schedule_tasks() {
    int assigned_any = 0;
    for (int c = 0; c < num_cores; ++c) {
        if (cores[c].assigned_task == NULL) {
            int best_idx = -1;
            int best_prio = -1;
            for (int i = 0; i < task_count; ++i) {
                if (tasks[i].state == READY) {
                    if (tasks[i].priority > best_prio ||
                       (tasks[i].priority == best_prio && tasks[i].id < tasks[best_idx].id)) {
                        best_prio = tasks[i].priority;
                        best_idx = i;
                    }
                }
            }
            if (best_idx != -1) {
                cores[c].assigned_task = &tasks[best_idx];
                tasks[best_idx].state = RUNNING;
                assigned_any = 1;
                printf("Scheduler: assigned Task %d (prio=%d) -> Core %d\n",
                       tasks[best_idx].id, tasks[best_idx].priority, c);
            }
        }
    }
    if (!assigned_any) printf("Scheduler: no READY tasks or no free cores.\n");
}

void run_simulation_step() {
    int progress = 0;
    for (int c = 0; c < num_cores; ++c) {
        if (cores[c].assigned_task) {
            Task *t = cores[c].assigned_task;
            if (t->state == RUNNING && t->remaining_ms > 0) {
                int step = (t->remaining_ms < TIME_STEP_MS) ? t->remaining_ms : TIME_STEP_MS;
                t->remaining_ms -= step;
                printf("[Core %d] Running Task %d (prio=%d) -> remaining %d ms\n",
                       c, t->id, t->priority, t->remaining_ms);
                progress = 1;

                if (t->remaining_ms <= 0) {
                    t->state = FINISHED;
                    cores[c].assigned_task = NULL;
                    printf("[Core %d] Task %d FINISHED\n", c, t->id);
                }
            }
        }
    }
    if (!progress) printf("No running tasks to advance.\n");
}

void save_log_csv() {
    FILE *f = fopen("run_log.csv", "w");
    if (!f) { printf("Cannot create run_log.csv\n"); return; }
    fprintf(f, "task_id,priority,remaining_ms,state\n");
    for (int i = 0; i < task_count; ++i) {
        fprintf(f, "%d,%d,%d,%d\n",
                tasks[i].id, tasks[i].priority, tasks[i].remaining_ms, tasks[i].state);
    }
    fclose(f);
    printf("Saved run_log.csv\n");
}

void print_menu() {
    printf("\nMenu:\n");
    printf("1 Create Task\n");
    printf("2 List Tasks\n");
    printf("3 Schedule Tasks (Priority)\n");
    printf("4 Show Cores Status\n");
    printf("5 Run Simulation Step (Advance 100 ms)\n");
    printf("6 Save log to CSV\n");
    printf("7 Exit\n");
    printf("Enter choice: ");
}

int main() {
    printf("=== CoreManager Phase 1 (Manual Simulation Mode) ===\n");
    printf("Enter number of simulated cores (1..%d) [default 2]: ", MAX_CORES);
    int n;
    if (scanf("%d", &n) == 1) {
        if (n < 1 || n > MAX_CORES) n = 2;
    } else { while(getchar()!='\n'); n = 2; }

    init_cores(n);

    int running = 1;
    while (running) {
        print_menu();
        int choice;
        if (scanf("%d", &choice) != 1) { while(getchar()!='\n'); continue; }
        switch (choice) {
            case 1: create_task_cli(); break;
            case 2: list_tasks(); break;
            case 3: schedule_tasks(); break;
            case 4: show_cores(); break;
            case 5: run_simulation_step(); break;
            case 6: save_log_csv(); break;
            case 7: running = 0; break;
            default: printf("Invalid choice.\n");
        }
    }

    printf("Exiting CoreManager.\n");
    return 0;
}