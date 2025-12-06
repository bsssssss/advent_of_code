#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void process_range(char* range)
{
    printf("range : %s\n", range);

    long start_id;
    long end_id;
    char** endptr;

    start_id = strtol(strsep(&range, "-"), endptr, 10);
    end_id   = strtol(range, endptr, 10);

    printf("start : %lu\n", start_id);
    printf("end   : %lu\n\n", end_id);
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
