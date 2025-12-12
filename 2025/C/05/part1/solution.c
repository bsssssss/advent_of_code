#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INPUT_FILE "test.txt"

typedef struct {
    int start;
    int end;
} Range;

typedef struct {
    Range ranges[200]; 
} Puzzle;

void die(char* message)
{
    if (errno) {
        perror(message);
    }
    else {
        printf("ERROR: %s\n", message);
    }
}

void read_input_file()
{
    FILE* fh = fopen(INPUT_FILE, "r");

    if (!fh) {
        die("could not open file");
    }

    char*   line = NULL;
    size_t  len = 0;
    ssize_t read;

    while ((read = getline(&line, &len, fh) != -1)) {

        int stage = 0;
        if (line[0] == '\n') {
            stage++;
        }

        if (stage == 0) {
            // parse ranges
        }
        else if (stage == 1) {
            // parse indices
        }
        else {
            die("oops");
        }

        line[strcspn(line, "\n")] = '\0';
    }

    fclose(fh);
    free(line);
}

int main(int argc, char* argv[])
{
    read_input_file();

    int fresh_count = 0;
    // printf("%d fresh ingredients\n", fresh_count);
    return 0;
}
