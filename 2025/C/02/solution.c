#include <stdio.h>

int main(int argc, char* argv[])
{
    FILE* file_handle;
    file_handle = fopen("input.txt", "r");

    if (file_handle == NULL) {
        printf("error: couldn't open file");
        return 1;
    }

    fclose(file_handle);
    return 0;
}
