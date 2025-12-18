#!/usr/bin/env bash

dir_count=$(find . -maxdepth 1 -type d | wc -l)
((dir_count -= 1))

dirname=$(printf "%.2d" $(($dir_count + 1)))/part1
mkdir -p $dirname

cat > $dirname/Makefile << "EOF"
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
EOF

cat > $dirname/solution.c << EOF
int main(void) 
{}
EOF

cat > $dirname/notes.md << EOF
# Challenge $dirname
EOF

touch $dirname/input.txt

cp "./utils/dbg.h" $dirname

echo -e "Created Advent of Code challenge in $dirname"
