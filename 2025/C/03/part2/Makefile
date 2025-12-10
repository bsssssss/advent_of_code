CFLAGS=-Wall -g -fsanitize=address -fsanitize=leak -fstack-protector-all

all: bin bin/solution

bin:
	mkdir -p bin

bin/solution: solution.c
	gcc $(CFLAGS) $< -o $@

clean:
	rm -rf bin
