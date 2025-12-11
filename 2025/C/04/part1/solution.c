#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INPUT_FILE "test.txt"

typedef struct {
    char* contents[200];
    int   rows;
    int   cols;
} Grid;

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

void Grid_free(Grid* grid)
{
    if (!grid) {
        return;
    }

    for (int i = 0; i < grid->rows; i++) {
        free(grid->contents[i]);
    }

    free(grid);
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

int is_roll(char c)
{
    if (c == '@') {
        return 1;
    }
    return 0;
}

int is_valid_cell(Grid* grid, int row, int col)
{
    if (row >= 0 && row < grid->rows && col >= 0 && col < grid->cols) {
        return 1;
    }
    return 0;
}

int count_rolls_around_cell(Grid* grid, int row, int col)
{
    int count = 0;
    return count;
}

int main(int argc, char* argv[])
{
    Grid* grid = read_file(INPUT_FILE);

    if (!grid) {
        return 1;
    }

    // Grid_print(grid);

    int accessible_rolls = 0;

    for (int row = 0; row < grid->rows; row++) {
        for (int col = 0; col < grid->cols; col++) {
            char cell = grid->contents[row][col];

            if (is_roll(cell)) {
                int roll_neighbors = 0;

                // TODO: count rolls around this cell

                if (roll_neighbors < 4) {
                    accessible_rolls++;
                }
            }
        }
    }

    printf("%d accessible rolls in total\n", accessible_rolls);

    Grid_free(grid);
    return 0;
}
