#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_CORES 16
#define MAX_TASKS 8
#define MONITOR_INTERVAL_MS 500

typedef enum { T_READY, T_RUNNING, T_FINISHED } TaskState;

typedef struct {
    int id;
    HANDLE thread;
    int pinned_core;
    volatile TaskState state;
    int work_ms;
} Task;

typedef struct {
    ULONGLONG IdleTime;
    ULONGLONG KernelTime;
    ULONGLONG UserTime;
} CORE_USAGE;

typedef LONG (WINAPI *NTQSI)(
    int SystemInformationClass,
    PVOID SystemInformation,
    ULONG SystemInformationLength,
    PULONG ReturnLength
);

typedef struct {
    LARGE_INTEGER IdleTime;
    LARGE_INTEGER KernelTime;
    LARGE_INTEGER UserTime;
    LARGE_INTEGER DpcTime;
    LARGE_INTEGER InterruptTime;
    ULONG InterruptCount;
} SYSTEM_PROCESSOR_PERFORMANCE_INFORMATION;

static NTQSI NtQuerySystemInformation = NULL;
static Task tasks[MAX_TASKS];
static int num_tasks = 0;
int get_ncores(void) {
    SYSTEM_INFO si;
    GetSystemInfo(&si);
    return (int)si.dwNumberOfProcessors;
}

int read_cpu_times(CORE_USAGE *info, int ncores) {
    SYSTEM_PROCESSOR_PERFORMANCE_INFORMATION data[MAX_CORES];
    ULONG len = 0;
    if (!NtQuerySystemInformation) return 0;
    LONG st = NtQuerySystemInformation(8, data, sizeof(data), &len);
    if (st != 0) return 0;
    for (int i = 0; i < ncores; ++i) {
        info[i].IdleTime   = data[i].IdleTime.QuadPart;
        info[i].KernelTime = data[i].KernelTime.QuadPart;
        info[i].UserTime   = data[i].UserTime.QuadPart;
    }
    return 1;
}

double calc_cpu_usage(const CORE_USAGE *prev, const CORE_USAGE *cur) {
    ULONGLONG idle_diff = cur->IdleTime - prev->IdleTime;
    ULONGLONG kernel_diff = cur->KernelTime - prev->KernelTime;
    ULONGLONG user_diff = cur->UserTime - prev->UserTime;
    ULONGLONG total = kernel_diff + user_diff;
    if (total == 0) return 0;
    double usage = 100.0 * (double)(total - idle_diff) / (double)total;
    if (usage < 0) usage = 0;
    if (usage > 100) usage = 100;
    return usage;
}

DWORD WINAPI task_func(LPVOID param) {
    Task *t = (Task *)param;
    if (t->pinned_core >= 0) {
        DWORD_PTR mask = ((DWORD_PTR)1) << t->pinned_core;
        SetThreadAffinityMask(GetCurrentThread(), mask);
    }
    t->state = T_RUNNING;
    DWORD end = GetTickCount() + t->work_ms;
    while (GetTickCount() < end) {
        for (volatile int i = 0; i < 1000000; ++i);
    }
    t->state = T_FINISHED;
    return 0;
}

void display_monitor(int ncores) {
    CORE_USAGE prev[MAX_CORES], cur[MAX_CORES];
    read_cpu_times(prev, ncores);
    Sleep(MONITOR_INTERVAL_MS);

    while (1) {
        read_cpu_times(cur, ncores);
        system("cls");
        printf("=== Phase 2: Real CPU Monitoring ===\n");
        printf("Cores detected: %d\n", ncores);
        printf("Core   Usage%%   Active Threads\n");
        printf("---------------------------------\n");
        int active[MAX_CORES] = {0};
        for (int i = 0; i < num_tasks; ++i) {
            if (tasks[i].state == T_RUNNING && tasks[i].pinned_core >= 0)
                active[tasks[i].pinned_core]++;
        }

        for (int i = 0; i < ncores; ++i) {
            double usage = calc_cpu_usage(&prev[i], &cur[i]);
            printf("CPU%-3d  %6.1f%%     %d\n", i, usage, active[i]);
            prev[i] = cur[i];
        }
        printf("\nRunning tasks:\n");
        for (int i = 0; i < num_tasks; ++i) {
            printf("Task %d: %s on core %d\n",
                   tasks[i].id,
                   (tasks[i].state == T_RUNNING ? "RUNNING" :
                   (tasks[i].state == T_FINISHED ? "FINISHED" : "READY")),
                   tasks[i].pinned_core);
        }
        Sleep(MONITOR_INTERVAL_MS);
    }
}

void setup_tasks(int ncores) {
    num_tasks = (ncores > MAX_TASKS ? MAX_TASKS : ncores);
    for (int i = 0; i < num_tasks; ++i) {
        tasks[i].id = i + 1;
        tasks[i].pinned_core = i % ncores;
        tasks[i].state = T_READY;
        tasks[i].work_ms = 4000 + (rand() % 2000); // simulate work 4–6 sec
        tasks[i].thread = CreateThread(NULL, 0, task_func, &tasks[i], 0, NULL);
    }
}

int main(void) {
    srand((unsigned)time(NULL));
    NtQuerySystemInformation = (NTQSI)GetProcAddress(GetModuleHandleA("ntdll.dll"),
                                                     "NtQuerySystemInformation");
    if (!NtQuerySystemInformation) {
        printf("Error: cannot load NtQuerySystemInformation.\n");
        return 1;
    }
    int ncores = get_ncores();
    if (ncores > MAX_CORES) ncores = MAX_CORES;
    setup_tasks(ncores);
    display_monitor(ncores);
    for (int i = 0; i < num_tasks; ++i)
        WaitForSingleObject(tasks[i].thread, INFINITE);
    return 0;

}
