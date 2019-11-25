# Set the compiler and arguments
CC = gcc
CFLAGS = -g -std=gnu90 -Wall -pedantic

# Compile all of these if make is done without arguments
all: bin/brightness bin/auto_background

# Create bin/brightness if the arg is brightness
brightness: bin/brightness
	$(info brightness done)

auto_background: bin/auto_background
	$(info auto_background done)

bin/brightness: brightness.c
	mkdir -p bin
	$(CC) -o bin/brightness $(CFLAGS) brightness.c

bin/auto_background: auto_background.c
	mkdir -p bin
	$(CC) -o bin/auto_background $(CFLAGS) auto_background.c

clean:
	rm -f bin/*
