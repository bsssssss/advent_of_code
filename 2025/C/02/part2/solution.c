#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LONG_STR 32
#define MAX_LINE_LEN 4096

bool is_invalid_id(long id)
{
    char id_str[MAX_LONG_STR];

    sprintf(id_str, "%ld", id);
    unsigned long length = strlen(id_str);

    if (length < 2) {
        return false;
    }

    printf("testing %s\n", id_str);

    return false;
}

long process_range(char* range)
{
    printf("range : %s\n", range);

    long start_id = strtol(strsep(&range, "-"), NULL, 10);
    long end_id = strtol(range, NULL, 10);
    long result = 0;

    for (long id = start_id; id <= end_id; id++) {
        if (is_invalid_id(id)) {
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

    char  buf_line[MAX_LINE_LEN];
    char* line = fgets(buf_line, sizeof(buf_line), file_handle);

    buf_line[strcspn(buf_line, "\n")] = '\0';

    const char* range_sep = ",";
    char*       range_token = strsep(&line, range_sep);

    long result = 0;

    while (range_token != NULL) {
        result += process_range(range_token);
        range_token = strsep(&line, range_sep);
    }

    printf("password: %ld\n", result);
    fclose(file_handle);

    return 0;
}
