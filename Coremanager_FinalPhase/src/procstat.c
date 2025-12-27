#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "procstat.h"

double get_cpu_usage() {
#ifdef __linux__
    FILE *fp = fopen("/proc/stat", "r");
    if (!fp) return 0;
    char cpu[5];
    long user, nice, system, idle;
    fscanf(fp, "%s %ld %ld %ld %ld", cpu, &user, &nice, &system, &idle);
    fclose(fp);
    double usage = 100.0 * (user + system) / (user + system + idle);
    return usage;
#else
    return 50.0; // placeholder for Windows
#endif
}

void log_cpu_usage(double usage) {
    FILE *fp = fopen("data/usage_log.csv", "a");
    if (fp) {
        fprintf(fp, "%.2f\n", usage);
        fclose(fp);
    }
}
