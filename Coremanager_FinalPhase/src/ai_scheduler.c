#include <stdio.h>
#include "ai_scheduler.h"

static double coeffs[4];

void load_model(const char *path) {
    FILE *fp = fopen(path, "r");
    if (!fp) {
        printf("[AI] Model file not found: %s\n", path);
        return;
    }
    for (int i = 0; i < 4; i++)
        fscanf(fp, "%lf", &coeffs[i]);
    fclose(fp);
    printf("[AI] Model loaded successfully.\n");
}

void adjust_priorities_from_ai(scheduler_t *sched) {
    load_model("data/model.txt");
    for (int i = 0; i < sched->task_count; i++) {
        double predicted = coeffs[0]*sched->tasks[i].priority + 20; // mock
        if (predicted > 70) sched->tasks[i].priority--;
        else sched->tasks[i].priority++;
    }
    printf("[AI] Adjusted priorities based on predicted load.\n");
}
