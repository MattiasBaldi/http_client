CC = gcc
CFLAGS = -Wall -Wextra -I src/unity/
UNITY_SRC = src/unity/unity.c
BUILD_DIR = build
TEST_DIR = tests

# Automatically find all test files
TEST_SOURCES = $(wildcard $(TEST_DIR)/test_*.c)
TEST_TARGETS = $(patsubst $(TEST_DIR)/test_%.c, $(BUILD_DIR)/test_%, $(TEST_SOURCES))

# Default target: build and run all tests
all: clean $(TEST_TARGETS)
	@echo "Running all tests..."
	@for test in $(TEST_TARGETS); do ./$$test; done

# Rule to build individual test executables
$(BUILD_DIR)/test_%: $(TEST_DIR)/test_%.c $(UNITY_SRC) | $(BUILD_DIR)
	@echo "Building $$@..."
	$(CC) $(CFLAGS) -o $@ $^

# Create build directory
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Clean build artifacts
clean:
	rm -rf $(BUILD_DIR)

.PHONY: all clean
