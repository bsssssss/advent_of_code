#include "dbg.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

#define INPUT_FILE "test.txt"
#define MAX_LEN 128

/*****************************************************************************/

typedef struct {
    char    op;
    int32_t nums[MAX_LEN];
    size_t  nums_len;
} Problem;

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

int32_t calculate_problem(Problem* pb)
{
    debug("calculating problem...");

    int32_t res = pb->nums[0];
    debug("\tnum 0: %d", res);

    for (size_t i = 1; i < pb->nums_len; i++) {
        int32_t curr = pb->nums[i];
        debug("\tnum %zu: %d", i, curr);

        if (pb->op == '+') {
            res += curr;
        }
        else if (pb->op == '*') {
            res *= curr;
        }
    }
    debug("-> %d\n", res);
    return res;
}

int32_t calculate(Puzzle* p)
{
    int32_t res = 0;

    Problem problems[MAX_LEN];
    size_t  problems_count = 0;

    size_t curr_nums_len = 0;
    for (int x = p->grid.dim.x - 1; x >= 0; x--) {

        int32_t curr_num = 0;
        char    curr_num_str[MAX_LEN];
        size_t  curr_num_str_len = 0;
        char    op               = '\0';
        size_t  empty_cell_count = 0;

        for (int y = 0; y < p->grid.dim.y; y++) {

            char curr_char = p->grid.contents[y][x];
            if (isdigit(curr_char)) {
                curr_num_str[curr_num_str_len]     = curr_char;
                curr_num_str[curr_num_str_len + 1] = '\0';
                curr_num_str_len++;
            }
            else if (curr_char == '+' || curr_char == '*') {
                op = curr_char;
            }
            else if (isblank(curr_char)) {
                empty_cell_count++;
            }
        }

        if (empty_cell_count == p->grid.dim.y) {
            res += calculate_problem(&problems[problems_count]);
            problems_count++;
            curr_nums_len = 0;
            continue;
        }

        curr_num = strtol(curr_num_str, NULL, 0);
        problems[problems_count].nums[curr_nums_len] = curr_num;
        problems[problems_count].nums_len            = curr_nums_len + 1;
        problems[problems_count].op                  = op;

        if (x == 0) {
            res += calculate_problem(&problems[problems_count]);
            problems_count++;
            curr_nums_len = 0;
            continue;
        }

        curr_nums_len++;
    }
    return res;
}

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
    for (size_t i = 0; i < p->raw_len; i++) {
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
    int32_t res = calculate(p);
    printf("result: %d\n", res);

    puzzle_destroy(p);
    return 0;

error:
    puzzle_destroy(p);
    return 1;
}
