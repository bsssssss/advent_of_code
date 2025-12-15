#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

#define INPUT_FILE "test.txt"

typedef struct {
    int* numbers;
    char operator;
} Problem;

typedef struct {
    Problem* problems;
    FILE* file;
} Puzzle;

void puzzle_close(Puzzle* puzzle);

void die(Puzzle* puzzle, char* message)
{
    if (puzzle) {
        puzzle_close(puzzle);
    }

    if (errno) {
        perror(message);
    }
    else {
        printf("ERROR: %s\n", message);
    }

    exit(1);
}

void puzzle_load(Puzzle* puzzle)
{

}

Puzzle* puzzle_open(char* filename)
{
    Puzzle* puzzle = malloc(sizeof(Puzzle));
    if (!puzzle) die(NULL, "could not malloc puzzle");

    puzzle->file = fopen(filename, "r");
    if (!puzzle->file) die(puzzle, "could not open input file");

    puzzle_load(puzzle);
    return puzzle;
}

void puzzle_close(Puzzle* puzzle)
{
    if (puzzle) {
        if (puzzle->file) fclose(puzzle->file);
        if (puzzle->problems) free(puzzle->problems);
        free(puzzle);
    }
}

int main(void)
{
    Puzzle* puzzle = puzzle_open(INPUT_FILE);

    puzzle_close(puzzle);
    return 0;
}
