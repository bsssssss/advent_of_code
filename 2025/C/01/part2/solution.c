#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char** argv)
{
    FILE* file = fopen(argv[1], "r");

    if (file == NULL) {
        fprintf(stderr, "could not open file\n");
        return 1;
    }

    char line[256];
    int  position   = 50;
    int  zero_count = 0;

    while (fgets(line, sizeof(line), file)) {
        if (line[0] == '\n') {
            continue;
        }

        char direction = line[0];
        char jump_by_str[strlen(line) + 1];
        int  jump_by;

        strcpy(jump_by_str, line + 1);
        jump_by = atoi(jump_by_str);

        if (direction == 'L') {
            for (int i = 0; i < jump_by; i++) {
                position = (position - 1) % 100;
                if (position == 0) {
                    zero_count++;
                }
            }
        }
        else if (direction == 'R') {
            for (int i = 0; i < jump_by; i++) {
                position = (position + 1) % 100;
                if (position == 0) {
                    zero_count++;
                }
            }
        }
    }

    fclose(file);
    printf("password is %d\n", zero_count);

    return 0;
}
