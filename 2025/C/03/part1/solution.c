#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Bank
{
    char* string;
    int   length;
} bank_t;

int max_value_position_in_array(int* array, int length)
{
    int maximum = 0;
    int position = 0;

    for (int i = 0; i < length; i++) {
        if (array[i] > maximum) {
            maximum = array[i];
            position = i;
        }
    }
    return position;
}

void bank_to_values(bank_t bank, int* values)
{
    for (int i = 0; i < bank.length; i++) {
        char str[2];
        strncpy(str, bank.string + i, 1);
        values[i] = atoi(str);
    }
}

int calculate_bank_joltage(bank_t bank, int n)
{
    printf("processing bank (%s)\n", bank.string);

    char joltage_str[n + 1];
    int  joltage = 0;

    for (int i = 0; i < n; i++) {
        int values[bank.length];
        bank_to_values(bank, values);

        int max_pos =
            max_value_position_in_array(values, bank.length - ((n - 1) - i));

        joltage_str[i] = bank.string[max_pos];
        bank.string = bank.string + (max_pos + 1);
        bank.length = strlen(bank.string);
    }

    joltage_str[n] = '\0';
    joltage = atoi(joltage_str);

    printf("\tjoltage for this bank is %d\n", joltage);
    printf("\n");

    return joltage;
}

int main(int argc, char* argv[])
{
    FILE* file_handle;
    file_handle = fopen("input.txt", "r");

    if (file_handle == NULL) {
        printf("error: couldn't open file\n");
        return 1;
    }

    char line[4096];
    int  result = 0;

    while (fgets(line, sizeof(line), file_handle)) {

        if (line[0] == '\n') {
            continue;
        }

        line[strcspn(line, "\n")] = '\0';

        bank_t bank;
        bank.string = strdup(line);
        bank.length = strlen(bank.string);

        int bank_joltage = calculate_bank_joltage(bank, 2);
        result += bank_joltage;
    }

    printf("total joltage: %d\n", result);

    fclose(file_handle);
    return 0;
}
