# Compiler
CC = g++

# Compiler flags
CFLAGS = -Wall -g

# Directories
SRC_DIR = src
TARGET_DIR = target

# Target executable
TARGET = $(TARGET_DIR)/hello_world

# Source files
SRCS = $(SRC_DIR)/main.cpp

# Object files
OBJS = $(SRCS:$(SRC_DIR)/%.cpp=$(TARGET_DIR)/%.o)

# Default target to compile the program
all: $(TARGET)

# Rule to build the executable
$(TARGET): $(OBJS)
	@mkdir -p $(TARGET_DIR)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

# Rule to compile source files into object files
$(TARGET_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(TARGET_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up compiled files
clean:
	rm -f $(OBJS) $(TARGET)
