#include <stdio.h>
#include <string.h>


int calculate_joltage(char* bank) 
{

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
        line[strcspn(line, "\n")] = '\0';
        printf("calculating joltage of %s\n", line);
    }

    printf("total joltage: %d\n", result);

    fclose(file_handle);
    return 0;
}
