# ============================================================
# Top-level Makefile — convenience wrapper for CMake builds
# ============================================================

# Build directory
BUILD_DIR := build

# CMake generator (optional override: make GEN="Ninja")
GEN ?= Unix Makefiles

# Build type (optional override: make TYPE=Debug)
TYPE ?= Debug


.PHONY: all configure build test clean distclean


all: build

# ------------------------------------------------------------
# Configure CMake
# ------------------------------------------------------------
configure:
	@mkdir -p $(BUILD_DIR)
	cd $(BUILD_DIR) && cmake -G "$(GEN)" -DCMAKE_BUILD_TYPE=$(TYPE) ..

# ------------------------------------------------------------
# Build everything
# ------------------------------------------------------------
build: configure
	cmake --build $(BUILD_DIR) --parallel

# ------------------------------------------------------------
# Run CTest
# ------------------------------------------------------------
test: build
	cd $(BUILD_DIR) && ctest --output-on-failure

# ------------------------------------------------------------
# Clean compiled objects (but keep build folder)
# ------------------------------------------------------------
clean:
	@echo "Cleaning build directory objects..."
	@if [ -d $(BUILD_DIR) ]; then cmake --build $(BUILD_DIR) --target clean; fi

# ------------------------------------------------------------
# Delete the entire build directory
# ------------------------------------------------------------
distclean:
	rm -rf $(BUILD_DIR)

