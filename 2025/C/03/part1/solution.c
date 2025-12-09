#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int calculate_bank_joltage(char* bank)
{
    int joltage = 0;
    int bank_length = strlen(bank);

    return joltage;
}

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
        // strip out new line char at the end
        line[strcspn(line, "\n")] = '\0';

        int bank_joltage = calculate_bank_joltage(line);
        result += bank_joltage;
    }

    printf("total joltage: %d\n", result);

    fclose(file_handle);
    return 0;
}
