#include "dbg.h"
#include <stdio.h>
#include <stdlib.h>

#define INPUT_FILE "test.txt"

typedef struct {
    char*  raw;
    size_t raw_len;
} Puzzle;

/*****************************************************************************/

int     read_input_file(Puzzle* pz, char* filename);
void    puzzle_destroy(Puzzle* pz);
Puzzle* puzzle_create(char* filename);

/*****************************************************************************/

int read_input_file(Puzzle* pz, char* filename)
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
    return 0;

error:
    if (fh) fclose(fh);
    if (pz->raw) free(pz->raw);
    return -1;
}

void puzzle_destroy(Puzzle* pz)
{
    if (pz) {
        if (pz->raw) free(pz->raw);
        free(pz);
    }
}

Puzzle* puzzle_create(char* filename)
{
    Puzzle* pz = malloc(sizeof(Puzzle));
    check_mem(pz);

    pz->raw = NULL;
    pz->raw_len = 0;

    read_input_file(pz, filename);
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
    Puzzle* pz = puzzle_create("bloo");
    check(pz, "failed to create puzzle");
    print_raw(pz);

    puzzle_destroy(pz);
    return 0;

error:
    return 1;
}
