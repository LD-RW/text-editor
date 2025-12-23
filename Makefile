CC = gcc
CFLAGS = -Wall -Wextra -pedantic -std=c99 -Iinclude
SRC_DIR = src
OBJ_DIR = obj

# Find all .c files in src directory
SRCS = $(wildcard $(SRC_DIR)/*.c)
# Convert .c filenames to .o filenames in obj directory
OBJS = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRCS))

# Name of the executable
TARGET = B-textEditor

# Default rule
all: $(TARGET)

# Link the object files to create the executable
$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET)

# Compile .c files to .o files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Create obj directory if it doesn't exist
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

# Clean up build files
clean:
	rm -rf $(OBJ_DIR) $(TARGET)

.PHONY: all clean