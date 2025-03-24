CARGO := cargo


C_DEMOS_BIN_DIR := demos/c
CPP_DEMOS_BIN_DIR := demos/cpp
RUST_SRC_DIR := demos/rust
BUILD_DIR := build
RUST_BIN := rust
MAKE := make
CMAKE := cmake

SEGFAULT_BIN := segfault 
SYSTEM_BIN := system 
SHM_VICTIM_BIN := victim
SHM_EXPLOIT_BIN := exploit 
ENV_VAR_BIN := env_var

# Default target: Compile and run all
all: build-all

# Compile all programs
build-c-stuff:
	$(CMAKE) -S . -B $(BUILD_DIR)
	$(CMAKE) --build $(BUILD_DIR)

# Compile Rust program
build-rust:
	@rustup default stable
	@cd $(RUST_SRC_DIR) && $(CARGO) build --release

build-all:
	$(MAKE) build-c-stuff
	$(MAKE) build-rust

run_rust:
	@echo -e "-- [Running Rust binary: $(RUST_BIN)]\n"
	@cd $(RUST_SRC_DIR) && $(CARGO) run

run_segfault:
	@echo -e "-- [Running C binary: $(SEGFAULT_BIN)]\n"
	@cd $(BUILD_DIR)/$(C_DEMOS_BIN_DIR)/ && ./$(SEGFAULT_BIN)

run_system:
	@echo -e "-- [Running C binary: $(SYSTEM_BIN)]\n"
	@cd $(BUILD_DIR)/$(C_DEMOS_BIN_DIR)/ && ./$(SYSTEM_BIN)

run_shm_victim:
	@echo -e "-- [Running C binary: $(SHM_VICTIM_BIN)]\n"
	@cd $(BUILD_DIR)/$(C_DEMOS_BIN_DIR)/ && ./$(SHM_VICTIM_BIN)

run_shm_exploit:
	@echo -e "-- [Running C binary: $(SHM_EXPLOIT_BIN)]\n"
	@cd $(BUILD_DIR)/$(C_DEMOS_BIN_DIR)/ && ./$(SHM_EXPLOIT_BIN)

run_env_var:
	@echo -e "-- [Running C++ binary: $(ENV_VAR_BIN)]\n"
	@cd $(BUILD_DIR)/$(CPP_DEMOS_BIN_DIR)/ && ./$(ENV_VAR_BIN)

# Clean up binaries
clean:
	@rm -rvf $(BUILD_DIR)
	@cd $(RUST_SRC_DIR) && $(CARGO) clean
	@echo -e "--> Cleaned up $(BUILD_DIR) and $(RUST_SRC_DIR)/target."
