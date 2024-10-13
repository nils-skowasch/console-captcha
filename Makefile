# Compiler for Linux
CC = g++

# Windows cross-compiler
WIN_CC = g++

# Compiler flags
CFLAGS = -Wall -g

# Directories
SRC_DIR = src
TARGET_DIR = target
CYGWIN_DIR = /cygdrive/c/cygwin64/bin

# Target executables
LINUX_TARGET = $(TARGET_DIR)/console-captcha
WIN_TARGET = $(TARGET_DIR)/console-captcha.exe

# Source files
SRCS = $(SRC_DIR)/main.cpp

# Object files
OBJS = $(SRCS:$(SRC_DIR)/%.cpp=$(TARGET_DIR)/%.o)
WIN_OBJS = $(SRCS:$(SRC_DIR)/%.cpp=$(TARGET_DIR)/%.win.o)

# Default target to compile the program for Linux
all: $(LINUX_TARGET)

# Rule to build the executable for Linux
$(LINUX_TARGET): $(OBJS)
	@mkdir -p $(TARGET_DIR)
	$(CC) $(CFLAGS) -o $(LINUX_TARGET) $(OBJS)

# Rule to compile source files into object files for Linux
$(TARGET_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(TARGET_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Target to cross-compile for Windows
win: $(WIN_TARGET) win-copy-dll 

# Rule to build the executable for Windows
$(WIN_TARGET): $(WIN_OBJS)
	@mkdir -p $(TARGET_DIR)
	$(WIN_CC) $(CFLAGS) -o $(WIN_TARGET) $(WIN_OBJS)

# Rule to compile source files into object files for Windows
$(TARGET_DIR)/%.win.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(TARGET_DIR)
	$(WIN_CC) $(CFLAGS) -c $< -o $@

# Copy required cygwin-dlls
win-copy-dll:
	cp $(CYGWIN_DIR)/cygwin1.dll $(TARGET_DIR)
	cp $(CYGWIN_DIR)/cygstdc++-6.dll $(TARGET_DIR)
	cp $(CYGWIN_DIR)/cyggcc_s-seh-1.dll $(TARGET_DIR)

# Clean up compiled files
clean:
	rm -f $(OBJS) $(WIN_OBJS) $(LINUX_TARGET) $(WIN_TARGET)

# Clean first and build afterwards for Linux
rebuild: clean $(LINUX_TARGET)

# Clean first and build afterwards for Windows
rebuild-win: clean win
