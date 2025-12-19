#include "dbg.h"
#include <stdio.h>
#include <stdlib.h>

#define INPUT_FILE "test.txt"
#define MAX_LEN 128

/*****************************************************************************/

typedef struct {
    size_t x;
    size_t y;
} Dim;

typedef struct {
    char contents[MAX_LEN][MAX_LEN];
    Dim  dim;
} Grid;

typedef struct {
    char*  raw;
    size_t raw_len;
    Grid   grid;
} Puzzle;

/*****************************************************************************/

void    read_input_file(Puzzle* p, char* filename);
void    puzzle_destroy(Puzzle* p);
Puzzle* puzzle_create(char* filename);

/*****************************************************************************/

void make_grid(Puzzle* p)
{
    char*  c    = p->raw;
    size_t rows = 0;
    size_t cols = 0;

    while (*c != '\0') {
        if (*c == '\n') {
            p->grid.contents[rows][cols] = '\0';
            rows++;
            cols = 0;
            c++;
            continue;
        }
        p->grid.contents[rows][cols]     = *c;
        p->grid.contents[rows][cols + 1] = '\0';

        p->grid.dim.y = rows + 1;
        p->grid.dim.x = cols + 1;
        cols++;
        c++;
    }
}

void read_input_file(Puzzle* p, char* filename)
{
    FILE* fh = fopen(filename, "rb");
    check(fh, "could not open file");

    fseek(fh, 0, SEEK_END);
    long sz = ftell(fh);
    rewind(fh);

    debug("size of %s: %ld bytes", filename, sz);

    // + 1 for null byte
    p->raw = malloc((sz * sizeof(char)) + 1);
    check_mem(p->raw);

    p->raw_len         = fread(p->raw, 1, sz, fh);
    p->raw[p->raw_len] = '\0';

    fclose(fh);
    return;

error:
    if (fh) fclose(fh);
    puzzle_destroy(p);
    return;
}

void print_raw(Puzzle* p)
{
    printf("\n");
    for (int i = 0; i < p->raw_len; i++) {
        printf("%c", p->raw[i]);
    }
    printf("\n");
}

Puzzle* puzzle_create(char* filename)
{
    Puzzle* p = malloc(sizeof(Puzzle));
    check_mem(p);

    p->raw     = NULL;
    p->raw_len = 0;

    p->grid.dim.x = 0;
    p->grid.dim.y = 0;

    read_input_file(p, filename);
    make_grid(p);

    return p;

error:
    puzzle_destroy(p);
    return NULL;
}

void puzzle_destroy(Puzzle* p)
{
    if (p) {
        if (p->raw) {
            free(p->raw);
        }
        free(p);
    }
}

int main(void)
{
    Puzzle* p = puzzle_create(INPUT_FILE);
    check(p, "failed to create puzzle");

    // print_raw(p);

    puzzle_destroy(p);
    return 0;

error:
    puzzle_destroy(p);
    return 1;
}
