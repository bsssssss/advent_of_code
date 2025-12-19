#include "dbg.h"
#include <stdio.h>
#include <stdlib.h>

#define INPUT_FILE "test.txt"
#define MAX_LEN 4096

/*****************************************************************************/

typedef struct {
    char** contents[MAX_LEN][MAX_LEN];
    size_t len;
} Grid;

typedef struct {
    char*  raw;
    size_t raw_len;
    Grid*  grid;
} Puzzle;

/*****************************************************************************/

void    read_input_file(Puzzle* p, char* filename);
void    puzzle_destroy(Puzzle* p);
Puzzle* puzzle_create(char* filename);

/*****************************************************************************/

void mk_grid(Puzzle* p)
{
    char cptr = *p->raw;
    while (cptr != '\0') {
        cptr++;
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

    p->raw_len          = fread(p->raw, 1, sz, fh);
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

    p->grid      = malloc(sizeof(Grid));
    p->grid->len = 0;

    read_input_file(p, filename);
    mk_grid(p);

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
        if (p->grid) {
            free(p->grid);
        }
        free(p);
    }
}


int main(void)
{
    Puzzle* p = puzzle_create(INPUT_FILE);
    check(p, "failed to create puzzle");

    print_raw(p);

    puzzle_destroy(p);
    return 0;

error:
    puzzle_destroy(p);
    return 1;
}
