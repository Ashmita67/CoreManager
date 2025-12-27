## Phase 1: Manual Multi-Core Scheduling Simulation

### Objective
To understand and implement core operating system concepts such as **process states, priority scheduling, multi-core execution, and time-based simulation** without relying on OS-level threads.

### Key Features
- Simulated multi-core CPU environment (1–16 cores)
- Task lifecycle management with states:
  - `READY`
  - `RUNNING`
  - `FINISHED`
- Priority-based scheduler
  - Highest-priority READY task assigned to free cores
  - Tie-breaking using task ID
- Discrete time simulation
  - Execution progresses in fixed **100 ms** time slices
- Interactive CLI-based control
  - Create tasks dynamically
  - Schedule tasks manually
  - Step-wise execution simulation
- Execution logging
  - Task status exported to CSV (`run_log.csv`) for analysis

### Concepts Demonstrated
- Process Control Blocks (PCBs)
- Non-preemptive priority scheduling
- Multi-core task assignment logic
- CPU burst simulation
- Kernel-like state transitions

### Outcome
Phase 1 establishes a **clear conceptual model of how an OS scheduler works internally**, making scheduling decisions visible and easy to reason about.

---

## Phase 2: Real CPU Core Monitoring & Thread Affinity (Windows)

### Objective
To bridge theory with reality by monitoring **actual CPU core utilization** and executing **real threads pinned to specific cores**.

### Key Features
- Detection of physical and logical CPU cores
- Creation of real OS threads using `CreateThread`
- Thread-to-core pinning using CPU affinity masks
- Real-time per-core CPU usage monitoring
- Native system call integration via:
  - `NtQuerySystemInformation`
- Live terminal dashboard showing:
  - CPU usage percentage per core
  - Active threads per core
  - Task execution states (`READY`, `RUNNING`, `FINISHED`)
- Continuous monitoring with periodic refresh

### Concepts Demonstrated
- Thread affinity and core pinning
- Kernel vs user time accounting
- OS-level performance counters
- Real-time monitoring loops
- Difference between simulated vs real scheduling

### Outcome
Phase 2 transforms the project into a **real systems-level tool**, closely mirroring how operating systems and profilers observe CPU behavior in practice.
