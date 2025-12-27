# stats.json Format Specification

This document describes the expected format for `data/stats.json` that the C backend should generate for the CoreManager 2.0 dashboard.

## Structure

```json
{
  "timestamp": "2024-01-15T10:30:45.123456",
  "energy_mode": "Balanced",
  "cores": [
    {
      "id": 0,
      "utilization": 45.5,
      "tasks": [
        {"id": 1, "priority": 5}
      ]
    },
    {
      "id": 1,
      "utilization": 62.3,
      "tasks": [
        {"id": 2, "priority": 8},
        {"id": 3, "priority": 3}
      ]
    }
  ],
  "tasks": [
    {
      "id": 1,
      "priority": 5,
      "deadline": 10.5,
      "core_assigned": 0,
      "completed": false,
      "cpu_time": 2.3
    },
    {
      "id": 2,
      "priority": 8,
      "deadline": 15.0,
      "core_assigned": 1,
      "completed": false,
      "cpu_time": 0.0
    }
  ],
  "migrations": [
    {
      "timestamp": "2024-01-15T10:30:40.123456",
      "task_id": 1,
      "from_core": 0,
      "to_core": 2
    }
  ]
}
```

## Field Descriptions

### Root Level
- `timestamp` (string, ISO 8601): Current system timestamp
- `energy_mode` (string): One of "Low", "Balanced", or "High"
- `cores` (array): Array of core objects
- `tasks` (array): Array of task objects
- `migrations` (array): Array of migration event objects

### Core Object
- `id` (integer): Core identifier (0, 1, 2, ...)
- `utilization` (float): CPU utilization percentage (0-100)
- `tasks` (array): Array of task references assigned to this core
  - Each task reference contains:
    - `id` (integer): Task ID
    - `priority` (integer): Task priority

### Task Object
- `id` (integer): Unique task identifier
- `priority` (integer): Task priority (higher = more important)
- `deadline` (float): Deadline in seconds
- `core_assigned` (integer): ID of the core this task is assigned to
- `completed` (boolean): Whether the task has completed
- `cpu_time` (float): Total CPU time used by this task in seconds

### Migration Object
- `timestamp` (string, ISO 8601): When the migration occurred
- `task_id` (integer): ID of the task that was migrated
- `from_core` (integer): Source core ID
- `to_core` (integer): Destination core ID

## Example C Code

```c
#include <stdio.h>
#include <time.h>
#include <json-c/json.h>

void write_stats_json(scheduler_t *sched, double *core_utilizations, int num_cores) {
    FILE *fp = fopen("data/stats.json", "w");
    if (!fp) return;
    
    fprintf(fp, "{\n");
    fprintf(fp, "  \"timestamp\": \"%s\",\n", get_current_timestamp());
    fprintf(fp, "  \"energy_mode\": \"%s\",\n", get_energy_mode_string());
    fprintf(fp, "  \"cores\": [\n");
    
    for (int i = 0; i < num_cores; i++) {
        fprintf(fp, "    {\n");
        fprintf(fp, "      \"id\": %d,\n", i);
        fprintf(fp, "      \"utilization\": %.2f,\n", core_utilizations[i]);
        fprintf(fp, "      \"tasks\": [\n");
        // Add tasks assigned to this core
        fprintf(fp, "      ]\n");
        fprintf(fp, "    }%s\n", i < num_cores - 1 ? "," : "");
    }
    
    fprintf(fp, "  ],\n");
    fprintf(fp, "  \"tasks\": [\n");
    // Add all tasks
    fprintf(fp, "  ],\n");
    fprintf(fp, "  \"migrations\": [\n");
    // Add migration events
    fprintf(fp, "  ]\n");
    fprintf(fp, "}\n");
    
    fclose(fp);
}
```

## Notes

- The dashboard reads this file every 3 seconds
- If the file doesn't exist, the dashboard will use default empty values
- All numeric values should be valid JSON numbers
- Timestamps should be in ISO 8601 format for consistency
- The dashboard gracefully handles missing fields



