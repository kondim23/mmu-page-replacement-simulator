SRCS = src/simulator.c src/pageTable.c src/memoryStructure.c
OBJS = build/simulator.o build/pageTable.o build/memoryStructure.o
CC = gcc
CFLAGS = -Iinclude
EXEC = bin/simulator

all: $(EXEC)

$(EXEC): $(OBJS) | bin
	$(CC) $(CFLAGS) -o $(EXEC) $(OBJS)

build/%.o: src/%.c | build
	$(CC) $(CFLAGS) -c $< -o $@

build:
	@mkdir -p build

bin:
	@mkdir -p bin

clean:
	rm -rf build bin