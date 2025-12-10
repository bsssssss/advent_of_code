#include <stdio.h>
#include <string.h>

int main(int argc, char* argv[]) 
{
    FILE* fh = fopen("input.txt", "r");

    if (fh == NULL) {
        printf("error: couldn't open file\n");
        return 1;
    }

    char line[4096];
    while (fgets(line, sizeof(line), fh)) {
        if (line[0] == '\n') {
            continue;
        }
        line[strcspn(line, "\n")] = '\0';
        printf("%s\n", line);
    }

    fclose(fh);
    return 0;
}
