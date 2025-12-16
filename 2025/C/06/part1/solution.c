#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

#define INPUT_FILE "test.txt"

typedef struct {
    FILE* input_file;
} Puzzle;

/**********************************************************************/

void die(Puzzle* p, char* message)
{
    if (errno) {
        perror(message);
    }
    else {
        printf("ERROR: %s\n", message);
    }
}

void puzz_close(Puzzle* p)
{
    if (p->input_file) {
        fclose(p->input_file);
    }
    if (p) {
        free(p);
    }
}

Puzzle* puzz_open(char* filename)
{
    Puzzle* p = malloc(sizeof(Puzzle));
    if (!p) die(p, "could not malloc puzzle");

    FILE* fh = fopen(filename, "r");
    if (!fh) die(p, "could not open file");

    p->input_file = fh;
    return p;
}

int main(void)
{
    Puzzle* p = puzz_open(INPUT_FILE);

    puzz_close(p);
    return 0;
}
