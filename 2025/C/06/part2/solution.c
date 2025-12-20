#include "dbg.h"
#include <ctype.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define INPUT_FILE "input.txt"
#define MAX_LEN 4096

/*****************************************************************************/

typedef struct {
    char    op;
    int64_t numbers[MAX_LEN];
    size_t  numbers_len;
} Problem;

typedef struct {
    size_t col;
    size_t row;
} Dim;

typedef struct {
    char contents[8][MAX_LEN];
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

int64_t calculate_problem(Problem* pb)
{
    int64_t res = pb->numbers[0];
    for (size_t i = 1; i < pb->numbers_len; i++) {
        int64_t curr = pb->numbers[i];
        if (pb->op == '+') {
            res += curr;
        }
        else if (pb->op == '*') {
            res *= curr;
        }
    }
    return res;
}

int64_t calculate(Puzzle* p)
{
    int64_t res = 0;

    Problem pb;
    pb.numbers_len = 0;

    // iterate over each rows (top to bottom) in each columns (right to left)
    for (int col = p->grid.dim.col - 1; col >= 0; col--) {

        // placeholder for the column's number
        char   num_str[64] = { '\0' };
        size_t num_str_len = 0;

        for (int row = 0; row < p->grid.dim.row; row++) {
            char curr_char = p->grid.contents[row][col];
            if (isdigit(curr_char)) {
                num_str[num_str_len] = curr_char;
                num_str_len++;
            }
            else if (curr_char == '+' || curr_char == '*') {
                pb.op = curr_char;
            }
        }

        num_str[num_str_len] = '\0';

        char*   endptr = NULL;
        int64_t num    = strtol(num_str, &endptr, 0);

        if (endptr != num_str) {
            pb.numbers[pb.numbers_len] = num;
            pb.numbers_len++;
        }

        if (endptr == num_str || col == 0) {
            res += calculate_problem(&pb);
            pb.numbers_len = 0;
        }
    }
    return res;
}

void make_grid(Puzzle* p)
{
    char*  curr_char = p->raw;
    size_t rows      = 0;
    size_t cols      = 0;

    while (*curr_char != '\0') {

        if (*curr_char == '\n') {
            p->grid.contents[rows][cols] = '\0';
            rows++;
            cols = 0;
            curr_char++;
            continue;
        }

        p->grid.contents[rows][cols]     = *curr_char;
        p->grid.contents[rows][cols + 1] = '\0';

        p->grid.dim.row = rows + 1;
        p->grid.dim.col = cols + 1;

        cols++;
        curr_char++;
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

    p->grid.dim.col = 0;
    p->grid.dim.row = 0;

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
    int64_t res = calculate(p);
    printf("result: %ld\n", res);

    puzzle_destroy(p);
    return 0;

error:
    puzzle_destroy(p);
    return 1;
}
