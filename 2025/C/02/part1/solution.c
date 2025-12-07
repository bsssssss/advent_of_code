#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int is_invalid_id(long id)
{
    char id_str[256];
    sprintf(id_str, "%ld", id);

    char c      = *id_str;
    int  length = 0;

    while (c != '\0') {
        length++;
        c = id_str[length];
    }

    if (length % 2 == 0) {
        char first_half_str[256];
        char second_half_str[256];

        strncpy(first_half_str, id_str, (length / 2));
        first_half_str[length / 2] = '\0';

        strncpy(second_half_str, id_str + (length / 2), (length / 2));
        second_half_str[length / 2] = '\0';

        long first_half  = strtol(first_half_str, NULL, 10);
        long second_half = strtol(second_half_str, NULL, 10);

        if (first_half == second_half) {
            return 0;
        }
    }
    return -1;
}

long process_range(char* range)
{
    printf("range : %s\n", range);

    long start_id = strtol(strsep(&range, "-"), NULL, 10);
    long end_id   = strtol(range, NULL, 10);
    long result   = 0;

    for (long id = start_id; id <= end_id; id++) {
        if (is_invalid_id(id) != -1) {
            result += id;
        }
    }
    return result;
}

int main(int argc, char* argv[])
{
    FILE* file_handle;
    file_handle = fopen("input.txt", "r");

    if (file_handle == NULL) {
        printf("error: couldn't open file");
        return 1;
    }

    char  buf_line[1024];
    char* line = fgets(buf_line, sizeof(buf_line), file_handle);
    buf_line[strcspn(buf_line, "\n")] = '\0';

    char* range_sep   = ",";
    char* range_token = strsep(&line, range_sep);
    long  result      = 0;

    while (range_token != NULL) {
        result += process_range(range_token);
        range_token = strsep(&line, range_sep);
    }

    printf("password: %ld\n", result);

    fclose(file_handle);
    return 0;
}
