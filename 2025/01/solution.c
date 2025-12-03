#include <stdio.h>
#include <string.h>

int main(int argc, char** argv) {
    // open the file passed as argument
    FILE* file = fopen(argv[1], "r");

    // file error handling
    if (file == NULL) {
        fprintf(stderr, "could not open file\n");
        return 1;
    }

    // init
    char line[256];
    int  position   = 50;
    int  zero_count = 0;
    int  line_count = 0;

    // parse lines
    while (fgets(line, sizeof(line), file)) {
        // skip empty lines
        if (line[0] == '\n') {
            continue;
        }

        // printf("line %d has %lu characters\n", line_count + 1, strlen(line));

        char direction = line[0];
        char value[20];

        strncpy(value, line + 1, strlen(line));
        value[strlen(line)] = '\0';

        printf("direction: %c, value: %s\n", direction, value);
        line_count++;
    }

    // close the file
    fclose(file);
    printf("Done.\n");

    return 0;
}
