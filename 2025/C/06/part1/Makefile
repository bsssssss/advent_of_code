OS := $(shell uname -s)

ifeq ($(OS), Darwin)
	CC := clang
	CFLAGS := -Wall -g -fsanitize=address
else
	CC := gcc
	CFLAGS := -Wall -g -fsanitize=address -fsanitize=leak
endif

all: bin bin/solution

bin:
	mkdir -p bin

bin/solution: solution.c
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf bin
