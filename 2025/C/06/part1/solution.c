#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

#define INPUT_FILE "test.txt"

typedef struct {
    int*   numbers;
    size_t len;
} NumRow;

typedef struct {
    char*  operators;
    size_t len;
} OpRow;

typedef struct {
    FILE*   file;
    NumRow* num_rows;
    OpRow   op_row;
} Puzzle;

/**********************************************************************/

void    die(Puzzle* p, char* message);
void    puzzle_close(Puzzle* p);
Puzzle* puzzle_open(char* filename);

/**********************************************************************/

void die(Puzzle* p, char* message)
{
    if (p) {
        puzzle_close(p);
    }

    if (errno) {
        perror(message);
    }
    else {
        printf("ERROR: %s\n", message);
    }
}

Puzzle* puzzle_open(char* filename)
{
    Puzzle* p = malloc(sizeof(Puzzle));
    if (!p) die(p, "could not malloc puzzle");

    FILE* fh = fopen(filename, "r");
    if (!fh) die(p, "could not open file");

    p->file = fh;

    return p;
}

void puzzle_close(Puzzle* p)
{
    if (p) {
        if (p->file) {
            fclose(p->file);
        }
        free(p);
    }
}

int main(void)
{
    Puzzle* p = puzzle_open(INPUT_FILE);

    puzzle_close(p);
    return 0;
}
