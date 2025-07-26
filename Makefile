# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -g

# Directories
OBJDIR = obj/
BINDIR = bin/
SRCDIR = src/
INSTALLDIR = /usr/local/bin/

# Output executable
EXECUTABLE = brightness-control
TARGET = $(BINDIR)$(EXECUTABLE)

# Source and object files
SRCS = $(wildcard $(SRCDIR)*.c)
OBJS = $(patsubst $(SRCDIR)%.c,$(OBJDIR)%.o,$(SRCS))

# Possible targets
all: $(OBJDIR) $(BINDIR) $(TARGET)

install: all 
	sudo cp $(TARGET) $(INSTALLDIR)

uninstall:
	sudo rm $(INSTALLDIR)$(EXECUTABLE)

# Ensure object directory exists
$(OBJDIR):
	mkdir -p $(OBJDIR)

# Ensure binary directory exists
$(BINDIR):
	mkdir -p $(BINDIR)

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

.PHONY: all clean install uninstall

