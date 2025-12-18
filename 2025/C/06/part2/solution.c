#include "dbg.h"
#include <stdio.h>
#include <stdlib.h>

#define INPUT_FILE "test.txt"
#define MAX_LEN 4096

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

void    read_input_file(Puzzle* pz, char* filename);
void    puzzle_destroy(Puzzle* pz);
Puzzle* puzzle_create(char* filename);

/*****************************************************************************/

void mk_grid(Puzzle* pz)
{
    char cptr = *pz->raw;
    while (cptr != '\0') {
        cptr++;
    }
}

void read_input_file(Puzzle* pz, char* filename)
{
    FILE* fh = fopen(filename, "rb");
    check(fh, "could not open file");

    fseek(fh, 0, SEEK_END);
    long sz = ftell(fh);
    rewind(fh);

    debug("size of %s: %ld bytes", filename, sz);

    // + 1 for null byte
    pz->raw = malloc((sz * sizeof(char)) + 1);
    check_mem(pz->raw);

    pz->raw_len          = fread(pz->raw, 1, sz, fh);
    pz->raw[pz->raw_len] = '\0';

    fclose(fh);
    return;

error:
    if (fh) fclose(fh);
    puzzle_destroy(pz);
    return;
}

void puzzle_destroy(Puzzle* pz)
{
    if (pz) {
        if (pz->raw) {
            free(pz->raw);
        }
        if (pz->grid) {
            free(pz->grid);
        }
        free(pz);
    }
}

Puzzle* puzzle_create(char* filename)
{
    Puzzle* pz = malloc(sizeof(Puzzle));
    check_mem(pz);

    pz->raw     = NULL;
    pz->raw_len = 0;

    pz->grid      = malloc(sizeof(Grid));
    pz->grid->len = 0;

    read_input_file(pz, filename);
    mk_grid(pz);

    return pz;

error:
    puzzle_destroy(pz);
    return NULL;
}

void print_raw(Puzzle* pz)
{
    printf("\n");
    for (int i = 0; i < pz->raw_len; i++) {
        printf("%c", pz->raw[i]);
    }
    printf("\n");
}

int main(void)
{
    Puzzle* pz = puzzle_create(INPUT_FILE);
    check(pz, "failed to create puzzle");

    print_raw(pz);

    puzzle_destroy(pz);
    return 0;

error:
    puzzle_destroy(pz);
    return 1;
}
