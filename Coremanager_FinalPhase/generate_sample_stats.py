#!/usr/bin/env python3
"""
Helper script to generate sample stats.json for testing the dashboard.
This can be used to simulate the C backend output.
"""

import json
import random
from datetime import datetime

def generate_sample_stats(num_cores=4, num_tasks=5):
    """Generate sample stats.json data"""
    
    cores = []
    tasks = []
    migrations = []
    
    # Generate cores
    for i in range(num_cores):
        core_tasks = []
        if i < num_tasks:
            core_tasks.append({"id": i + 1, "priority": random.randint(1, 10)})
        
        cores.append({
            "id": i,
            "utilization": random.uniform(20, 80),
            "tasks": core_tasks
        })
    
    # Generate tasks
    for i in range(num_tasks):
        tasks.append({
            "id": i + 1,
            "priority": random.randint(1, 10),
            "deadline": random.uniform(5, 20),
            "core_assigned": random.randint(0, num_cores - 1),
            "completed": random.choice([True, False]),
            "cpu_time": random.uniform(0, 5)
        })
    
    # Generate some migration events
    for i in range(3):
        migrations.append({
            "timestamp": datetime.now().isoformat(),
            "task_id": random.randint(1, num_tasks),
            "from_core": random.randint(0, num_cores - 1),
            "to_core": random.randint(0, num_cores - 1)
        })
    
    # Determine energy mode based on average utilization
    avg_util = sum(c["utilization"] for c in cores) / len(cores)
    if avg_util < 30:
        energy_mode = "Low"
    elif avg_util > 70:
        energy_mode = "High"
    else:
        energy_mode = "Balanced"
    
    stats = {
        "timestamp": datetime.now().isoformat(),
        "energy_mode": energy_mode,
        "cores": cores,
        "tasks": tasks,
        "migrations": migrations
    }
    
    return stats

if __name__ == "__main__":
    import os
    
    # Ensure data directory exists
    os.makedirs('data', exist_ok=True)
    
    # Generate and save sample stats
    stats = generate_sample_stats()
    
    with open('data/stats.json', 'w') as f:
        json.dump(stats, f, indent=2)
    
    print("Generated sample stats.json")
    print(f"   - {len(stats['cores'])} cores")
    print(f"   - {len(stats['tasks'])} tasks")
    print(f"   - {len(stats['migrations'])} migrations")
    print(f"   - Energy mode: {stats['energy_mode']}")

