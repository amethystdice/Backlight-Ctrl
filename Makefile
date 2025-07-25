# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -g

# Output executable
TARGET = i-brightness-control

# Source and object files
SRCS = main.c error-handler.c
OBJS = $(SRCS:.c=.o)

# Default target
all: $(TARGET)

# Link object files to create executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

# Compile source files to object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean build artifacts
clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean
