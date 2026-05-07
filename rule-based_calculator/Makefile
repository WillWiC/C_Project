# Variables
CC      = gcc
CFLAGS  = -Wall -Wextra -I./include

SRC_DIR   = src
TEST_DIR  = tests
BUILD_DIR = build

# Source files and objects
SRCS    = $(SRC_DIR)/main.c $(SRC_DIR)/parser.c $(SRC_DIR)/engine.c
OBJS    = $(BUILD_DIR)/main.o $(BUILD_DIR)/parser.o $(BUILD_DIR)/engine.o

# Test files and objects
TEST_SRCS = $(TEST_DIR)/test_engine.c $(SRC_DIR)/parser.c $(SRC_DIR)/engine.c
TEST_OBJS = $(BUILD_DIR)/test_engine.o $(BUILD_DIR)/parser.o $(BUILD_DIR)/engine.o

TARGET      = calculator
TEST_TARGET = test_runner

# ── Default target ──────────────────────────────────────────
all: $(BUILD_DIR) $(TARGET)

# Link main executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

# Compile each source file into build/
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/test_engine.o: $(TEST_DIR)/test_engine.c
	$(CC) $(CFLAGS) -c $< -o $@

# ── Test target ──────────────────────────────────────────────
test: $(BUILD_DIR) $(TEST_TARGET)
	./$(TEST_TARGET)

$(TEST_TARGET): $(TEST_OBJS)
	$(CC) $(CFLAGS) -o $@ $^

# ── Utility targets ──────────────────────────────────────────
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

clean:
	rm -rf $(BUILD_DIR) $(TARGET) $(TEST_TARGET)

.PHONY: all test clean