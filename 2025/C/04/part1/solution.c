#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INPUT_FILE "input.txt"

typedef struct {
    char* contents[200];
    int   rows;
    int   cols;
} Grid;

void Grid_free(Grid* grid)
{
    if (!grid) return;

    for (int i = 0; i < grid->rows; i++) {
        free(grid->contents[i]);
    }

    free(grid);
}

int Grid_add_row(Grid* grid, char* row)
{
    if (grid->rows >= 200) {
        printf("grid is full (max 200 rows\n");
        return -1;
    }

    grid->contents[grid->rows] = strdup(row);

    if (!grid->contents[grid->rows]) {
        printf("error: couldn't add row to Grid\n");
        return -1;
    }

    if (grid->cols == 0) {
        grid->cols = strlen(row);
    }

    grid->rows++;

    return 0;
}

void Grid_print(Grid* grid)
{
    for (int i = 0; i < grid->rows; i++) {
        printf("%s\n", grid->contents[i]);
    }
}

Grid* Grid_create()
{
    Grid* grid = malloc(sizeof(Grid));
    if (!grid) {
        printf("error: couldn't allocate memory for Grid\n");
        return NULL;
    }

    grid->rows = 0;
    grid->cols = 0;

    return grid;
}

Grid* read_file(char* file)
{
    FILE* fh = fopen(file, "r");
    if (fh == NULL) {
        printf("error: couldn't open file\n");
        return NULL;
    }

    Grid* grid = Grid_create();

    if (!grid) {
        return NULL;
    }

    char buffer[4096];
    while (fgets(buffer, sizeof(buffer), fh)) {

        if (buffer[0] == '\n') {
            continue;
        }

        buffer[strcspn(buffer, "\n")] = '\0';

        if (Grid_add_row(grid, buffer) != 0) {
            Grid_free(grid);
            fclose(fh);
            return NULL;
        }
    }

    fclose(fh);
    return grid;
}

int main(int argc, char* argv[])
{
    Grid* grid = read_file(INPUT_FILE);

    if (!grid) {
        return 1;
    }

    Grid_print(grid);

    Grid_free(grid);
    return 0;
}
