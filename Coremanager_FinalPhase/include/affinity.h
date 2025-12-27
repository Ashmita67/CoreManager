#ifndef AFFINITY_H
#define AFFINITY_H

#include <pthread.h>

int get_core_count();
void print_core_info();
void bind_thread_to_core(pthread_t thread, int core_id);

#endif
