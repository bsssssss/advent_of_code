#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LONG_STR 32
#define MAX_LINE_LEN 4096

/*
 * prend un mot de <n> charactères,
 * le divise en <divisor> mots de <n> / <divisor> charactères
 * retourne un tableau de ces mots (transformés en nombres)
 *
 * le tableau est initialisé par le caller, et on recupere 
 * son pointeur comme argument
 */

long* chunk_id(char* id_str, long divisor, long* output_array)
{
    long length = strlen(id_str);

    for (long i = 0; i < divisor; i++) {
        char chunk_str[MAX_LONG_STR];
        long chunk;
        long chunk_size = length / divisor;

        strncpy(chunk_str, id_str + (i * chunk_size), chunk_size);

        // Always ensure string is null terminated
        chunk_str[chunk_size] = '\0';

        chunk = strtol(chunk_str, NULL, 10);
        // printf("chunk %d: %ld\n", i, chunk);

        output_array[i] = chunk;
    }

    return output_array;
}

bool is_invalid_id(long id)
{
    char id_str[MAX_LONG_STR];
    sprintf(id_str, "%ld", id);

    unsigned long length = strlen(id_str);
    bool result = false;

    // ids of length 1 are always valid
    if (length < 2) {
        return result;
    }

    for (long d = 2; d <= length; d++) {
        if (length % d == 0) {
            long chunked[d];
            // printf("testing %s, divided in %ld chunks\n", id_str, d);
            long* seq_arr = chunk_id(id_str, d, chunked);

            result = true;
            for (long i = 1; i < d; i++) {
                if (seq_arr[i] != seq_arr[0]) {
                    result = false;
                    break;
                }
            }

            if (result) {
                // printf("found invalid id: %ld\n", id);
                return result;
            }
        }
    }

    return result;
}

long process_range(char* range)
{
    // printf("range : %s\n", range);

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

    char buf_line[MAX_LINE_LEN];
    char* line = fgets(buf_line, sizeof(buf_line), file_handle);

    buf_line[strcspn(buf_line, "\n")] = '\0';

    const char* range_sep = ",";
    char* range_token = strsep(&line, range_sep);

    long result = 0;

    while (range_token != NULL) {
        result += process_range(range_token);
        range_token = strsep(&line, range_sep);
    }

    printf("password: %ld\n", result);
    fclose(file_handle);

    return 0;
}
