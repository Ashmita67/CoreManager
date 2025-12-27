# ======================================================
#  CoreManager 2.0 — Multi-Module OS Scheduler Simulator
#  Compatible with Linux / WSL
# ======================================================

# Compiler and flags
CC = gcc
CFLAGS = -Iinclude -lpthread -Wall

# Source and output files
SRC = $(wildcard src/*.c)
OUT = coremanager

# ---------------------------
# Build the complete program
# ---------------------------
all:
	$(CC) $(SRC) $(CFLAGS) -o $(OUT)
	@echo "✅ Build complete! Run with: ./$(OUT)"

# ---------------------------
# Run the compiled executable
# ---------------------------
run: all
	@echo "🚀 Running CoreManager..."
	@./$(OUT)

# ---------------------------
# Clean up compiled binaries
# ---------------------------
clean:
	@rm -f $(OUT)
	@echo "🧹 Cleaned up build files."
