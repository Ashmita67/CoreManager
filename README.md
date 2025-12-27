# CoreManager  - AI-Driven CPU Scheduling Simulator

**CoreManager** is an intelligent operating system scheduler simulator that demonstrates how an AI-driven system can manage multiple processes across different CPU cores. The project combines a high-performance C backend for real-time scheduling simulation with a modern web-based dashboard for visualization and monitoring.

### Core Innovation
The system simulates intelligent process scheduling by:
- **Deciding which process runs where** - Dynamic core assignment based on priority, deadline, and load
- **When to migrate tasks** - Load balancing through task migration between cores
- **Energy conservation** - Adaptive energy modes (Low/Balanced/High) based on system load
- **AI-powered prediction** - Machine learning model predicts future load for proactive scheduling

---

## 🏗️ System Architecture

### High-Level Overview

```
┌─────────────────────────────────────────────────────────────┐
│                    CoreManager System                       │
├─────────────────────────────────────────────────────────────┤
│                                                             │
│  ┌──────────────┐         ┌──────────────┐                  │
│  │  C Backend   │────────▶│  Data Files  │                  | 
│  │  (Scheduler) │         │  (JSON/CSV)  │                  │
│  └──────────────┘         └──────────────┘                  │
│         │                          │                        │
│         │                          │                        │
│         ▼                          ▼                        │
│  ┌──────────────────────────────────────────┐               │
│  │     Python Dashboard (Dash/Plotly)       │               │
│  │     Real-time Visualization & Monitoring │               │
│  └──────────────────────────────────────────┘               │
│                                                             │
└─────────────────────────────────────────────────────────────┘
```

---
## 🚀 Key Features & Innovations

### 1. **Multi-Policy Scheduling**
- Supports priority-based, deadline-based, and AI-assisted scheduling
- Can switch between policies dynamically

### 2. **Intelligent Load Balancing**
- Automatic task migration when cores become overloaded
- Maintains balanced utilization across all cores

### 3. **Energy Efficiency**
- Adaptive energy modes based on system load
- Consolidates tasks to minimize active cores in low-load scenarios

### 4. **AI-Powered Predictions**
- Machine learning model predicts future CPU load
- Enables proactive scheduling decisions
- Reduces response time through predictive task placement

### 5. **Real-time Monitoring**
- Live dashboard with sub-5-second update latency
- Comprehensive visualization of all system components
- Historical trend analysis

### 6. **Multi-threaded Architecture**
- Uses POSIX threads (pthreads) for concurrent task execution
- CPU affinity binding for realistic core assignment
- Thread-safe operations

---

## 💻 Technical Stack

### Backend
- **Language**: C (C11 standard)
- **Libraries**: 
  - POSIX Threads (pthread) for multi-threading
  - Standard I/O for file operations
- **Platform**: Linux/WSL (with Windows compatibility layer)
- **Build System**: Makefile

### Frontend
- **Language**: Python 3.8+
- **Framework**: Dash 2.14+
- **Visualization**: Plotly 5.18+
- **Data Processing**: Pandas 2.1+
- **Machine Learning**: Scikit-learn 1.3+

### Data Format
- **JSON**: For structured system state
- **CSV**: For time-series utilization data
- **Text**: For model coefficients

---

## Dashboard Features

- 📈 **Real-time CPU Core Utilization** - Bar charts showing utilization for each core
- 🔧 **Core Panels** - Individual panels for each CPU core showing assigned tasks and load
- 📋 **Task Management Table** - Complete view of all tasks with priority, deadline, and status
- 🔄 **Migration Events Log** - Real-time log of task migrations between cores
- ⚡ **Energy Mode Indicator** - Visual display of current energy mode (Low/Balanced/High)
- 🤖 **AI-Predicted Load Trend** - Time series graph with AI predictions for future load
---

## 📊 Use Cases & Applications

### 1. **Educational Purpose**
- Demonstrates OS scheduling concepts
- Shows real-world multi-core CPU management
- Illustrates AI integration in system software

### 2. **Research & Development**
- Testbed for new scheduling algorithms
- Performance analysis of different policies
- Energy efficiency optimization studies

### 3. **System Monitoring**
- Real-time visualization of CPU utilization
- Task distribution analysis
- Migration pattern observation

### 4. **AI/ML Integration**
- Demonstrates ML in system-level software
- Predictive scheduling research
- Load forecasting applications

---

## 🏃 How to Run

### Prerequisites
- **C Compiler**: GCC with pthread support
- **Python 3.8+**: With pip package manager
- **Linux/WSL**: For full CPU affinity support (Windows has limited support)

### Building the Backend

```bash
# Compile the C program
make

# Or manually:
gcc src/*.c -Iinclude -lpthread -Wall -o coremanager
```

### Running the Backend

```bash
# Run the scheduler simulation
./coremanager

# Or using Makefile:
make run
```

### Setting Up the Dashboard

```bash
# Install Python dependencies
pip install -r requirements.txt

# Generate sample data (if backend not running)
python generate_sample_stats.py

# Start the dashboard
python app.py
```

### Accessing the Dashboard

Open your web browser and navigate to:
```
http://127.0.0.1:8050
```

---

## 🚀 Quick Start

```bash
# Build backend
make

# Run simulation
./coremanager

# Start dashboard (separate terminal)
pip install -r requirements.txt
python app.py

# Open browser: http://127.0.0.1:8050
```

---
## Troubleshooting

- **No data showing**: Ensure `data/stats.json` and/or `data/usage_log.csv` exist
- **Port already in use**: Change the port in `app.py` (line with `app.run_server`)
- **Missing dependencies**: Run `pip install -r requirements.txt`