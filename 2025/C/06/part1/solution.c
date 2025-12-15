#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INPUT_FILE "test.txt"

typedef struct {
    char* contents;
    int   is_numbers;
    int   is_operators;
} Row;

typedef struct {
    int*   numbers;
    size_t numbers_count;
    char   operator;
    int    result;
} Problem;

typedef struct {
    FILE*    file;
    char*    contents;
    size_t   contents_count;
    Row*     rows;
    size_t   rows_count;
    Problem* problems;
    size_t   problems_count;
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

char* get_operators(Puzzle* p)
{}

int* get_numbers(Puzzle* p)
{}

size_t count_rows(Puzzle* p)
{
    rewind(p->file);

    size_t line_count = 0;
    char*  line_buffer = NULL;
    size_t sz = 0;

    while (getline(&line_buffer, &sz, p->file) != -1) {
        line_count++;
    }
    free(line_buffer);

    return line_count;
}

void register_rows(Puzzle* p)
{
    rewind(p->file);

    char*  line_buffer = NULL;
    size_t sz = 0;
    size_t count = 0;

    while (getline(&line_buffer, &sz, p->file) != -1) {
        line_buffer[strcspn(line_buffer, "\n")] = '\0';

        size_t str_len = strlen(line_buffer) + 1;

        p->rows[count].contents = malloc(str_len);
        if (!p->rows[count].contents) {
            die(p, "failed to allocate memory for row");
        }

        strncpy(p->rows[count].contents, line_buffer, str_len);

        if (count < p->rows_count - 1) {
            p->rows[count].is_numbers = 1;
        }
        else {
            p->rows[count].is_operators = 1;
        }

        count++;
    }
    free(line_buffer);
}

void print_rows(Puzzle* p)
{
    if (p->rows) {
        for (int i = 0; i < p->rows_count; i++) {
            if (p->rows[i].is_numbers) {
                printf("%s ---> numbers\n", p->rows[i].contents);
            }
            else if (p->rows[i].is_operators) {
                printf("%s ---> operators\n", p->rows[i].contents);
            }
        }
    }
}

void print_contents(Puzzle* p)
{
    if (p->contents) {
        for (int i = 0; i < p->contents_count; i++) {
            printf("%c", p->contents[i]);
        }
    }
}

void register_contents(Puzzle* p)
{
    fseek(p->file, 0, SEEK_END);
    size_t fc = ftell(p->file);

    p->contents = malloc(fc * sizeof(char));
    if (!p->contents) {
        die(p, "failed to malloc puzzle contents");
    }

    p->contents_count = fc;

    rewind(p->file);
    size_t rc = fread(p->contents, fc, 1, p->file);
    if (!rc) {
        die(p, "failed to read input file contents");
    }
}

Puzzle* puzzle_create(char* filename)
{
    Puzzle* p = malloc(sizeof(Puzzle));
    if (!p) {
        die(NULL, "could not malloc puzzle");
    }

    p->file = fopen(filename, "r");
    if (!p->file) {
        die(p, "could not open input file");
    }

    p->problems = NULL;
    p->problems_count = 0;

    size_t rows_count = count_rows(p);
    p->rows = malloc(rows_count * sizeof(Row));
    p->rows_count = rows_count;

    for (int i = 0; i < p->rows_count; i++) {
        p->rows[i].is_numbers = 0;
        p->rows[i].is_operators = 0;
    }

    register_contents(p);
    register_rows(p);

    return p;
}

void puzzle_close(Puzzle* p)
{
    if (p) {
        if (p->file) {
            fclose(p->file);
        }
        if (p->contents) {
            free(p->contents);
        }
        if (p->rows) {
            for (int i = 0; i < p->rows_count; i++) {
                free(p->rows[i].contents);
            }
            free(p->rows);
        }
        if (p->problems) {
            // TODO: free all problems first
            free(p->problems);
        }
        free(p);
    }
}

int main(void)
{
    Puzzle* p = puzzle_create(INPUT_FILE);

    // print_contents(p);
    // print_rows(p);

    puzzle_close(p);
    return 0;
}
