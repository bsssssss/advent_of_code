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

void    die(Puzzle* p, char* message);
void    puzzle_close(Puzzle* p);
void    puzzle_parse(Puzzle* p);
void    parse_line(Puzzle* p, char* line, size_t linecount);
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

void parse_op_line(OpRow* op_row, char* line)
{
    char* nptr = line;
    while (*nptr) {
        if (*nptr == '+' || *nptr == '*') {
            op_row->operators[op_row->len] = *nptr;
            op_row->len++;
        }
        nptr++;
    }
}

void parse_num_line(NumRow* num_row, char* line)
{
    char* nptr = line;
    char* endptr = NULL;

    while (*nptr) {
        int64_t val = strtoll(nptr, &endptr, 0);
        if (nptr != endptr) {
            num_row->numbers[num_row->len] = val;
            nptr = endptr;
            num_row->len++;
        }
        else {
            nptr++;
        }
    }
}

void print_numbers(Puzzle* p)
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

void print_operators(Puzzle* p)
{
    printf("operators: ");
    for (int i = 0; i < p->op_row.len; i++) {
        printf("%c ", p->op_row.operators[i]);
    }
    printf("\n\n");
}

int is_operators_line(char* line)
{
    char* p = NULL;
    int   n;

    n = sscanf(line, "%m[+*]", &p);

    // only free if sscanf is successful
    if (n == 1) {
        free(p);
        return n;
    }

    return n;
}

int is_numbers_line(char* line)
{
    int p;
    int n;
    n = sscanf(line, "%u", &p);
    return n == 1;
}

void puzzle_parse(Puzzle* p)
{
    char*   lineptr = NULL;
    size_t  len = 0;
    ssize_t read;

    while ((read = getline(&lineptr, &len, p->file) != -1)) {

        if (is_numbers_line(lineptr)) {
            if (p->num_rows) {
                NumRow* current = &p->num_rows[p->num_rows_len];

                current->capacity = 4096;
                current->numbers = malloc(current->capacity * sizeof(int64_t));
                current->len = 0;

                parse_num_line(current, lineptr);
                p->num_rows_len++;
            }
            else {
                die(p, "num_rows is not allocated");
            }
        }

        else if (is_operators_line(lineptr)) {
            OpRow* current = &p->op_row;
            parse_op_line(current, lineptr);
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

    p->num_rows_capacity = 4096;
    p->num_rows = malloc(p->num_rows_capacity * sizeof(NumRow));
    if (!p->num_rows) {
        die(p, "could not allocate num_rows");
    }
    p->num_rows_len = 0;

    p->op_row.capacity = 4096;
    p->op_row.operators = malloc(p->op_row.capacity * sizeof(char));
    if (!p->op_row.operators) {
        die(p, "could not allocate operators in op_row");
    }
    p->op_row.len = 0;

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

int64_t calculate_total(Puzzle* p)
{
    int64_t res = 0;

    for (int col = 0; col < p->op_row.len; col++) {
        int64_t curr = p->num_rows[0].numbers[col];

        char op = p->op_row.operators[col];
        for (int row = 1; row < p->num_rows_len; row++) {
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
