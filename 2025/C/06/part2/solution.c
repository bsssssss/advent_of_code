#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INPUT_FILE "input.txt"

typedef struct {
    int64_t* numbers;
    size_t   len;
    size_t   capacity;
} NumRow;

typedef struct {
    char*  operators;
    size_t len;
    size_t capacity;
} OpRow;

typedef struct {
    FILE* file;

    NumRow* num_rows;
    size_t  num_rows_len;
    size_t  num_rows_capacity;

    OpRow op_row;
} Puzzle;

/**********************************************************************/

void die(Puzzle* p, char* message);

Puzzle* puzzle_open(char* filename);
void    puzzle_close(Puzzle* p);

void puzzle_parse(Puzzle* p);
void parse_line(Puzzle* p, char* line, size_t linecount);
void parse_operators(OpRow* op_row, const char* line);
void parse_numbers(NumRow* num_row, const char* line);

void print_operators(const Puzzle* p);
void print_numbers(const Puzzle* p);

int is_operators_line(const char* line);
int is_numbers_line(const char* line);

int64_t calculate_total(const Puzzle* p);

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

    exit(1);
}

void parse_operators(OpRow* op_row, const char* line)
{
    const char* char_ptr = line;
    while (*char_ptr) {
        if (*char_ptr == '+' || *char_ptr == '*') {
            op_row->operators[op_row->len] = *char_ptr;
            op_row->len++;
        }
        char_ptr++;
    }
}

void parse_numbers(NumRow* num_row, const char* line)
{
    const char* nptr   = line;
    char*       endptr = NULL;

    while (*nptr) {
        int64_t val = strtoll(nptr, &endptr, 0);

        if (nptr != endptr) {
            if (num_row->len >= num_row->capacity) {
                die(NULL, "too many numbers in row");
            }
            num_row->numbers[num_row->len] = val;

            nptr = endptr;
            num_row->len++;
        }
        else {
            nptr++;
        }
    }
}

void print_numbers(const Puzzle* p)
{
    for (int i = 0; i < p->num_rows_len; i++) {
        printf("line %d: ", i + 1);

        for (int j = 0; j < p->num_rows[i].len; j++) {
            printf("%ld ", p->num_rows[i].numbers[j]);
        }
        printf("\n");
    }
    printf("\n\n");
}

void print_operators(const Puzzle* p)
{
    printf("operators: ");
    for (int i = 0; i < p->op_row.len; i++) {
        printf("%c ", p->op_row.operators[i]);
    }
    printf("\n\n");
}

int is_operators_line(const char* line)
{
    while (*line == ' ' || *line == '\t') {
        line++;
    }
    return *line == '+' || *line == '*';
}

int is_numbers_line(const char* line)
{
    while (*line == ' ' || *line == '\t') {
        line++;
    }
    return *line >= '0' && *line <= '9';
}

void puzzle_parse(Puzzle* p)
{
    char*   lineptr = NULL;
    size_t  len     = 0;
    ssize_t read;

    while ((read = getline(&lineptr, &len, p->file) != -1)) {
        if (is_numbers_line(lineptr)) {
            if (p->num_rows_len >= p->num_rows_capacity) {
                die(p, "too many numbers rows");
            }

            NumRow* current = &p->num_rows[p->num_rows_len];

            current->len      = 0;
            current->capacity = 4096;
            current->numbers  = malloc(current->capacity * sizeof(int64_t));

            parse_numbers(current, lineptr);
            p->num_rows_len++;
        }
        else if (is_operators_line(lineptr)) {
            OpRow* current = &p->op_row;
            parse_operators(current, lineptr);
        }
    }

    free(lineptr);
}

Puzzle* puzzle_open(char* filename)
{
    Puzzle* p = malloc(sizeof(Puzzle));
    if (!p) {
        die(p, "could not malloc puzzle");
    }

    FILE* fh = fopen(filename, "r");
    if (!fh) {
        die(p, "could not open file");
    }
    p->file = fh;

    p->num_rows_len      = 0;
    p->num_rows_capacity = 4096;
    p->num_rows          = malloc(p->num_rows_capacity * sizeof(NumRow));

    if (!p->num_rows) {
        die(p, "could not allocate num_rows");
    }

    p->op_row.len       = 0;
    p->op_row.capacity  = 4096;
    p->op_row.operators = malloc(p->op_row.capacity * sizeof(char));

    if (!p->op_row.operators) {
        die(p, "could not allocate operators in op_row");
    }

    return p;
}

void puzzle_close(Puzzle* p)
{
    if (p) {
        if (p->file) {
            fclose(p->file);
        }

        if (p->num_rows) {
            for (int i = 0; i < p->num_rows_len; i++) {
                if (p->num_rows[i].numbers) {
                    free(p->num_rows[i].numbers);
                }
            }
            free(p->num_rows);
        }

        if (p->op_row.operators) {
            free(p->op_row.operators);
        }

        free(p);
    }
}

int64_t calculate_total(const Puzzle* p)
{
    int64_t res = 0;

    for (size_t col = 0; col < p->op_row.len; col++) {
        int64_t curr = p->num_rows[0].numbers[col];

        char op = p->op_row.operators[col];
        for (size_t row = 1; row < p->num_rows_len; row++) {
            int64_t num = p->num_rows[row].numbers[col];
            if (op == '+') {
                curr += num;
            }
            else if (op == '*') {
                curr *= num;
            }
        }

        res += curr;
    }

    return res;
}

int main(void)
{
    Puzzle* p = puzzle_open(INPUT_FILE);
    puzzle_parse(p);

    int64_t total = calculate_total(p);
    printf("%ld\n", total);

    puzzle_close(p);
    return 0;
}
