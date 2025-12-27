#ifndef UTILS_H
#define UTILS_H

#include <time.h>

double get_time_diff(struct timespec start, struct timespec end);
void log_message(const char *msg);

#endif
