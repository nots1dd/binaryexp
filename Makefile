# Compiler and flags
GCC := gcc
CXX := g++
CXXFLAGS := -Wall -Wextra -fno-stack-protector -z execstack

RUSTC := rustc
JAVAC := javac
GO := go

DEMO_DIR := demos
CXX_SRC_DIR := $(DEMO_DIR)/cpp
C_SRC_DIR := $(DEMO_DIR)/c
RUST_SRC_DIR := $(DEMO_DIR)/rust

# Source files
C_SRC := $(C_SRC_DIR)/segfault.c
CXX_SRC := $(CXX_SRC_DIR)/env_var.cpp
RUST_SRC := $(RUST_SRC_DIR)/src/main.rs

# Output binaries
C_BIN := segfault
CXX_BIN := env_var
RUST_BIN := integer_overflow

# Default target: Compile and run all
all: build run

# Compile all programs
build: $(C_BIN) $(CXX_BIN) $(RUST_BIN) $(JAVA_BIN) $(GO_BIN)

$(C_BIN): $(C_SRC)
	$(GCC) $(C_SRC) -o $(C_SRC_DIR)/$(C_BIN)

# Compile C++ program
$(CXX_BIN): $(CXX_SRC)
	$(CXX) $(CXXFLAGS) -o $(CXX_SRC_DIR)/$(CXX_BIN) $<

# Compile Rust program
$(RUST_BIN): $(RUST_SRC)
	@cd $(RUST_SRC_DIR) && cargo build --release

# Run all binaries
run: run_cpp run_rust

run_cpp:
	@echo -e "-- [Running C++ binary: $(CXX_BIN)]"
	./$(CXX_SRC_DIR)/$(CXX_BIN)

run_rust:
	@echo -e "-- [Running Rust binary: $(RUST_BIN)]"
	./$(RUST_BIN)

# Clean up binaries
clean:
	@rm -f $(C_SRC_DIR)/$(C_BIN) $(CXX_SRC_DIR)/$(CXX_BIN) $(RUST_SRC_DIR)/$(RUST_BIN)
