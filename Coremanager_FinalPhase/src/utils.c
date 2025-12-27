#include <stdio.h>
#include <time.h>
#include "utils.h"

double get_time_diff(struct timespec start, struct timespec end) {
    return (end.tv_sec - start.tv_sec) +
           (end.tv_nsec - start.tv_nsec) / 1e9;
}

void log_message(const char *msg) {
    FILE *fp = fopen("data/log.txt", "a");
    if (fp) {
        fprintf(fp, "%s\n", msg);
        fclose(fp);
    }
}
