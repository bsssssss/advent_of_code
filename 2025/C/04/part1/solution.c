#include <stdint.h>
#include <stdio.h>
#include <string.h>

typedef struct Grid_row
{
    const char* symbols;
    int         length;
} row_t;

int calculate_accessible(row_t row)
{
    uint32_t result = 0;
    return result;
}

int main(int argc, char* argv[])
{
    FILE* fh = fopen("input.txt", "r");

    if (fh == NULL) {
        printf("error: couldn't open file\n");
        return 1;
    }

    char     line[4096];
    uint32_t result = 0;

    while (fgets(line, sizeof(line), fh)) {
        if (line[0] == '\n') {
            continue;
        }

        line[strcspn(line, "\n")] = '\0';

        row_t row;
        row.symbols = line;
        row.length = strlen(line);

        uint32_t accessible_rolls = calculate_accessible(row);
        result += accessible_rolls;
    }

    fclose(fh);
    return 0;
}
