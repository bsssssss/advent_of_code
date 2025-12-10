#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Bank
{
    const char* string;
    int         length;
} bank_t;

int bank_get_digit(bank_t bank, int offset)
{
    if (offset < 0 || offset >= bank.length) {
        printf("error: trying to get out of bonds digit from bank\n");
        return -1;
    }

    return bank.string[offset] - '0';
}

int find_maximum(bank_t bank, int start, int end)
{
    int maximum = 0;
    int maximum_offset = 0;

    for (int i = start; i < end; i++) {
        int value = bank_get_digit(bank, i);
        if (value > maximum) {
            maximum = value;
            maximum_offset = i;
        }
    }

    return maximum_offset;
}

uint64_t calculate_bank_joltage(bank_t bank, int n_batteries)
{
    if (n_batteries > bank.length) {
        printf("error: not enough batteries in bank\n");
        return -1;
    }

    char joltage_str[n_batteries + 1];

    int start = 0;
    int end = bank.length - (n_batteries - 1);

    for (int i = 0; i < n_batteries; i++) {
        int max_offset = find_maximum(bank, start, end);
        joltage_str[i] = bank.string[max_offset];

        start = max_offset + 1;
        end += 1;
    }

    joltage_str[n_batteries] = '\0';
    uint64_t joltage = strtoll(joltage_str, NULL, 10);

    return joltage;
}

bank_t bank_create(char* line)
{
    bank_t bank;
    bank.string = line;
    bank.length = strlen(line);

    return bank;
}

int main(int argc, char* argv[])
{
    FILE* file_handle;
    file_handle = fopen("input.txt", "r");

    if (file_handle == NULL) {
        printf("error: couldn't open file\n");
        return 1;
    }

    char     line[4096];
    uint64_t result = 0;

    while (fgets(line, sizeof(line), file_handle)) {

        if (line[0] == '\n') {
            continue;
        }

        line[strcspn(line, "\n")] = '\0';

        bank_t   bank = bank_create(line);
        uint64_t bank_joltage = calculate_bank_joltage(bank, 12);

        if (bank_joltage == -1) {
            return 1;
        }

        result += bank_joltage;
    }

    printf("total joltage: %lu\n", result);
    fclose(file_handle);

    return 0;
}
