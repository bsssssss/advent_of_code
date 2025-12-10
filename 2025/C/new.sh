#!/usr/bin/env bash

dir_count=$(find . -maxdepth 1 -type d | wc -l)
((dir_count -= 1))

dirname=$(printf "%.2d" $(($dir_count + 1)))/part1
mkdir -p $dirname

cat > $dirname/Makefile << EOF
CFLAGS=-Wall -g -fsanitize=address -fsanitize=leak

all: bin bin/solution

bin:
	mkdir -p bin

bin/solution: solution.c
	gcc \$(CFLAGS) \$< -o \$@

clean:
	rm -rf bin
EOF

cat > $dirname/solution.c << EOF
#include <stdio.h>

int main(int argc, char* argv[]) 
{
}
EOF

cat > $dirname/notes.md << EOF
# Challenge $dirname
EOF

touch $dirname/input.txt

echo -e "Created Advent of Code challenge in $dirname"
