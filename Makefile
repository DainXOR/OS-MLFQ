CC = gcc
BASE_FLAGS = -Wall -Wextra
COMP_FLAGS = -Werror -Wshadow -Wconversion -Wnull-dereference
ALL_FLAGS = $(BASE_FLAGS) -Wpedantic $(COMP_FLAGS)


CFLAGS = $(ALL_FLAGS) -std=c11 -Iinclude
SRC_DIR = src
OBJ_DIR = build
TARGET = mlfq

# Auto-discover source files
SRC = $(shell find $(SRC_DIR) -name "*.c")

# Map src/foo.c -> build/foo.o
OBJ = $(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

# Dependency files (.d)
DEP = $(OBJ:.o=.d)

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) $^ -o $@

# Compile rule with dependency generation
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -MMD -MP -c $< -o $@

run: $(TARGET)
	./$(TARGET)

clean:
	rm -rf $(OBJ_DIR) $(TARGET)

re: clean all

debug: CFLAGS += -g -fno-omit-frame-pointer -O0
debug: re

valgrind: debug
	valgrind --leak-check=full ./$(TARGET)

# Include dependency files if they exist
-include $(DEP)

.PHONY: all clean re run debug valgrind
