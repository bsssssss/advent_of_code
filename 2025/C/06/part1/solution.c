#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INPUT_FILE "input.txt"

typedef struct {
    int    numbers[4096];
    size_t count;
} NumRow;

typedef struct {
    char*  operators[4096];
    size_t count;
} OpRow;

typedef struct {
    char* contents;
    char  type;
} Row;

typedef struct {
    int*   numbers;
    size_t numbers_count;
    char   operator;
    int    result;
} Problem;

typedef struct {
    FILE* file;

    char*  contents;
    size_t contents_count;

    Row*   rows;
    size_t rows_count;

    NumRow* numbers_rows;
    OpRow*  operators_row;

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

size_t puzzle_get_linecount(Puzzle* p)
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
            p->rows[count].type = 'n';

            char *str_p = line_buffer, *endptr = NULL;
            int   ncount = 0;
            while (*str_p) {
                long val = strtol(str_p, &endptr, 0);
                if (str_p != endptr) {
                    if (!errno) {
                        char ascii[4096] = "";
                        strncpy(ascii, str_p, endptr - str_p);
                        ascii[endptr - str_p] = '\0';
                        p->numbers_rows[count].numbers[ncount] = val;
                        ncount++;
                        p->numbers_rows[count].count = ncount;
                    }
                    str_p = endptr;
                }
                for (; *str_p; str_p++) {
                    if ('0' <= *str_p && *str_p <= '9') {
                        break;
                    }
                    if ((*str_p == '+' || *str_p == '-') && '0' <= *(str_p + 1) && *(str_p + 1) <= '9') {
                        break;
                    }
                }
            }
        }
        else {
            p->rows[count].type = 'o';

            char *str_p = line_buffer, *endptr = NULL;
            int   ncount = 0;
            while (*str_p != '\0') {
                if (str_p != endptr) {
                    if (!errno) {
                        char operator = *str_p;
                        p->operators_row->operators[ncount] = str_p;
                        ncount++;
                        p->operators_row->count = ncount;
                    }
                    str_p++;
                }
                for (; *str_p; str_p++) {
                    if (*str_p == '+' || *str_p == '*') {
                        break;
                    }
                }
            }
        }

        count++;
    }
    free(line_buffer);
}

void print_rows(Puzzle* p)
{
    if (p->rows) {
        for (int i = 0; i < p->rows_count; i++) {
            if (p->rows[i].type == 'n') {
                printf("%s ---> numbers (%zu)\n", p->rows[i].contents, p->numbers_rows[i].count);
            }
            else if (p->rows[i].type == 'o') {
                printf("%s ---> operators (%zu)\n", p->rows[i].contents, p->operators_row->count);
            }
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

void print_contents(Puzzle* p)
{
    if (p->contents) {
        for (int i = 0; i < p->contents_count; i++) {
            printf("%c", p->contents[i]);
        }
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

    size_t rows_count = puzzle_get_linecount(p);

    p->rows = malloc(rows_count * sizeof(Row));
    p->rows_count = rows_count;
    for (int i = 0; i < p->rows_count; i++) {
        p->rows[i].type = '\0';
    }

    p->numbers_rows = malloc((rows_count - 1) * sizeof(NumRow));
    p->operators_row = malloc(sizeof(OpRow));

    p->problems = NULL;
    p->problems_count = 0;

    // register_contents(p);
    register_rows(p);

    return p;
}

void puzzle_close(Puzzle* p)
{
    if (p) {

        if (p->file) {
            fclose(p->file);
        }

        // if (p->contents) {
        //     free(p->contents);
        // }

        if (p->rows) {
            for (int i = 0; i < p->rows_count; i++) {
                free(p->rows[i].contents);
            }
            free(p->rows);
        }

        if (p->numbers_rows) {
            // for (int i = 0; i < p->numbers_rows->count; i++) {
            //     if (p->numbers_rows[i].numbers) {
            //         free(p->numbers_rows[i].numbers);
            //     }
            // }
            free(p->numbers_rows);
        }

        if (p->operators_row) {
            // for (int i = 0; i < p->operators_row->count; i++) {
            //     free(p->operators_row[i].operators);
            // }
            // free(p->operators_row);
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
    print_rows(p);

    // for (int i = 0; i < p->rows_count - 1; i++) {
    //     for (int n = 0; i < p->numbers_rows[i].count; n++) {
    //         printf("%d ", p->numbers_rows[i].numbers[n]);
    //     }
    //     printf("\n");
    // }

    puzzle_close(p);
    return 0;
}
