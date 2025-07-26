# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -g

# Directories
OBJDIR = obj/
BINDIR = bin/
SRCDIR = src/

# Output executable
TARGET = $(BINDIR)i-brightness-control

# Source and object files
SRCS = $(wildcard $(SRCDIR)*.c)
OBJS = $(patsubst $(SRCDIR)%.c,$(OBJDIR)%.o,$(SRCS))

# Default target
all: $(OBJDIR) $(TARGET)

# Ensure object directory exists
$(OBJDIR):
	mkdir -p $(OBJDIR)
	mkdir -p $(BINDIR)
	mkdir -p $(SRCDIR)

# Link object files into the final executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

# Compile .c source files to .o object files
$(OBJDIR)%.o: $(SRCDIR)%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Remove build artifacts
clean:
	rm -f $(OBJS) $(TARGET)
	rmdir  $(OBJDIR)
	rmdir  $(BINDIR)

.PHONY: all clean

