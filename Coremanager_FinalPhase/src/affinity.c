#define _GNU_SOURCE
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <sched.h>
#include "affinity.h"

int get_core_count() {
    return sysconf(_SC_NPROCESSORS_ONLN);
}

void print_core_info() {
    int cores = get_core_count();
    printf("[Affinity] Detected %d CPU cores available.\n", cores);
}

void bind_thread_to_core(pthread_t thread, int core_id) {
    cpu_set_t set;
    CPU_ZERO(&set);
    CPU_SET(core_id, &set);

    int result = pthread_setaffinity_np(thread, sizeof(cpu_set_t), &set);
    if (result != 0) {
        perror("[Affinity] Error setting core binding");
    } else {
        cpu_set_t get;
        CPU_ZERO(&get);
        pthread_getaffinity_np(thread, sizeof(cpu_set_t), &get);

        printf("[Affinity] Thread bound to core %d successfully. Active mask: ", core_id);
        for (int i = 0; i < get_core_count(); i++)
            if (CPU_ISSET(i, &get))
                printf("%d ", i);
        printf("\n");
    }
}
