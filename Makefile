CC = gcc
OPENSSL_DIR = /opt/homebrew/opt/openssl
CFLAGS = -Wall -Wextra -I$(OPENSSL_DIR)/include
LDFLAGS = -L$(OPENSSL_DIR)/lib -lssl -lcrypto
BUILD_DIR = build
TEST_DIR = tests

# Test-specific settings
TEST_CFLAGS = $(CFLAGS) -I $(TEST_DIR)/unity -DTEST
UNITY_SRC = $(TEST_DIR)/unity/unity.c
TEST_SOURCES = $(wildcard $(TEST_DIR)/test_*.c)
TEST_TARGETS = $(patsubst $(TEST_DIR)/test_%.c, $(BUILD_DIR)/test_%, $(TEST_SOURCES))

# Default: build main and run tests
all: main test

# Build main program
main: main.c
	$(CC) $(CFLAGS) $(LDFLAGS) -o main main.c

# Build and run tests
test: $(TEST_TARGETS)
	@echo "Running tests..."
	@for test in $(TEST_TARGETS); do ./$$test; done

# Build individual test executables (compile main.c functions, not main())
$(BUILD_DIR)/test_%: $(TEST_DIR)/test_%.c main.c $(UNITY_SRC) | $(BUILD_DIR)
	$(CC) $(TEST_CFLAGS) $(LDFLAGS) -o $@ $^

# Create build directory
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Clean build artifacts
clean:
	rm -rf $(BUILD_DIR) main

# Run the main program
run: main
	./main $(ARGS)

.PHONY: all main test clean run
