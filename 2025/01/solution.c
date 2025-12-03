#include <stdio.h>

int main(int argc, char **argv) {

    // open the file passed as argument
    FILE *file = fopen(argv[1], "r");

    // file error handling
    if (file == NULL) {
        fprintf(stderr, "could not open file\n");
        return 1;
    }

    // init
    char line[256];
    int position = 50;
    int zero_counter = 0;

    // parse lines
    while (fgets(line, sizeof(line), file)) {
        // skip empty lines
        if (line[0] == '\n') {
            continue;
        }
        // it's easy to get the first character in the line
        char direction = line[0];
        // printf("direction: %c\n", direction);

        char character;
    }

    // close the file
    fclose(file);

    return 0;
}
