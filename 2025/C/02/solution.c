#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void process_id(long id)
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
        char first_half[128];
        char second_half[128];
        strncpy(first_half, id_str, (length / 2) - 1);
        strncpy(second_half, id_str + (length / 2) - 1, (length / 2) - 1);

        printf("first: %s, second: %s\n", first_half, second_half);
    }
}

void process_range(char* range)
{

    printf("range : %s\n", range);

    long start_id = strtol(strsep(&range, "-"), NULL, 10);
    long end_id   = strtol(range, NULL, 10);

    printf("start : %ld\n", start_id);
    printf("end   : %ld\n", end_id);

    for (long id = start_id; id <= end_id; id++) {
        process_id(id);
    }
}

int main(int argc, char* argv[])
{
    FILE* file_handle;
    file_handle = fopen("test.txt", "r");

    if (file_handle == NULL) {
        printf("error: couldn't open file");
        return 1;
    }

    char  buf_line[1024];
    char* line = fgets(buf_line, sizeof(buf_line), file_handle);

    buf_line[strcspn(buf_line, "\n")] = '\0';

    char* range_token;
    char* range_sep = ",";

    range_token = strsep(&line, range_sep);
    while (range_token != NULL) {
        process_range(range_token);
        range_token = strsep(&line, range_sep);
    }

    fclose(file_handle);
    return 0;
}
