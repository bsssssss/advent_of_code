#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int find_position_for_maximum_in_array(int* arr, int len)
{
    int max = 0;
    int max_position = 0;
    for (int i = 0; i < len; i++) {
        printf("battery %d, value %d\n", i, arr[i]);
        if (arr[i] > max) {
            max = arr[i];
            max_position = i;
        }
    }

    printf("maximum val is at pos %d\n", max_position);

    return max_position;
}

int calculate_joltage(int* bank, int len)
{
    int joltage = 0;
    int first_battery_position = find_position_for_maximum_in_array(bank, len - 1);
    int second_battery_position = find_position_for_maximum_in_array(bank + (first_battery_position + 1), (len - (first_battery_position + 1)));
    printf("max value for first battery is %d\n", bank[first_battery_position]);
    printf("max value for second battery is %d\n", bank[second_battery_position + (first_battery_position + 1)]);

    return joltage;
}

int* line_to_array_of_int(char* line, int* arr, int len)
{
    for (int i = 0; i < len; i++) {
        char c[2];
        c[0] = line[i];
        c[1] = '\0';
        arr[i] = atoi(c);
    }
    return arr;
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
        int arr_int[256];
        line[strcspn(line, "\n")] = '\0';

        printf("calculating joltage of %s...\n", line);
        result += calculate_joltage(line_to_array_of_int(line, arr_int, strlen(line)), strlen(line));
        printf("\n");
    }

    printf("total joltage: %d\n", result);

    fclose(file_handle);
    return 0;
}
