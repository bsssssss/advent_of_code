#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char* argv[])
{
    FILE* file_handle;
    file_handle = fopen("test.txt", "r");

    if (file_handle == NULL) {
        printf("error: couldn't open file\n");
        return 1;
    }

    char line[4096];
    int result = 0;

    while (fgets(line, sizeof(line), file_handle)) {
        int arr_int[256];

        // strip out new line char at the end
        line[strcspn(line, "\n")] = '\0';

        printf("calculating joltage of %s...\n", line);
    }

    printf("total joltage: %d\n", result);

    fclose(file_handle);
    return 0;
}
