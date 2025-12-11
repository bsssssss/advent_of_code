#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INPUT_FILE "input.txt"

typedef struct {
    char* cells[200];
    int   rows;
    int   cols;
} Grid;

int is_roll_cell(char c)
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

    // left
    if (is_valid_cell(grid, row, col - 1)) {
        if (is_roll_cell(grid->cells[row][col - 1])) {
            count++;
        }
    }
    // right
    if (is_valid_cell(grid, row, col + 1)) {
        if (is_roll_cell(grid->cells[row][col + 1])) {
            count++;
        }
    }
    // up left
    if (is_valid_cell(grid, row - 1, col - 1)) {
        if (is_roll_cell(grid->cells[row - 1][col - 1])) {
            count++;
        }
    }
    // up right
    if (is_valid_cell(grid, row - 1, col + 1)) {
        if (is_roll_cell(grid->cells[row - 1][col + 1])) {
            count++;
        }
    }
    // down left
    if (is_valid_cell(grid, row + 1, col - 1)) {
        if (is_roll_cell(grid->cells[row + 1][col - 1])) {
            count++;
        }
    }
    // down right
    if (is_valid_cell(grid, row + 1, col + 1)) {
        if (is_roll_cell(grid->cells[row + 1][col + 1])) {
            count++;
        }
    }
    // up
    if (is_valid_cell(grid, row - 1, col)) {
        if (is_roll_cell(grid->cells[row - 1][col])) {
            count++;
        }
    }
    // down
    if (is_valid_cell(grid, row + 1, col)) {
        if (is_roll_cell(grid->cells[row + 1][col])) {
            count++;
        }
    }

    return count;
}

int remove_accessible_rolls(Grid* grid)
{
    int removed = 0;

    for (int row = 0; row < grid->rows; row++) {
        for (int col = 0; col < grid->cols; col++) {
            char cell = grid->cells[row][col];

            if (is_roll_cell(cell)) {
                int roll_neighbors = count_rolls_around_cell(grid, row, col);
                if (roll_neighbors < 4) {
                    removed++;
                    grid->cells[row][col] = 'x';
                }
            }
        }
    }

    return removed;
}

int Grid_add_row(Grid* grid, char* row)
{
    if (grid->rows >= 200) {
        printf("grid is full (max 200 rows\n");
        return -1;
    }

    grid->cells[grid->rows] = strdup(row);

    if (!grid->cells[grid->rows]) {
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
        printf("%s\n", grid->cells[i]);
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
        free(grid->cells[i]);
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

int main(int argc, char* argv[])
{
    Grid* grid = read_file(INPUT_FILE);

    if (!grid) {
        return 1;
    }

    int removed_rolls = 0;

    bool keep_going = true;
    while (keep_going) {
        int accessible_rolls = remove_accessible_rolls(grid);
        if (accessible_rolls <= 0) {
            keep_going = false;
        }
        else {
            removed_rolls += accessible_rolls;
        }
    }

    printf("%d rolls removed in total\n", removed_rolls);

    Grid_free(grid);
    return 0;
}
