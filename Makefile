# Set the compiler and arguments
CC = gcc
CFLAGS = -g -std=gnu90 -Wall -pedantic

# Compile all of these if make is done without arguments
all: bin/brightness

# Create bin/brightness if the arg is brightness
brightness: bin/brightness
	$(info Brightness done)

bin/brightness: brightness.c
	mkdir -p bin
	$(CC) -o bin/brightness $(CFLAGS) brightness.c

clean:
	rm -f bin/*

# Create bin/brightness if the arg is brightness
#brightness: bin/brightness

