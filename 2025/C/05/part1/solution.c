#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INPUT_FILE "input.txt"
#define MAX_ROWS 2048

typedef struct {
    uint64_t start;
    uint64_t end;
} Range;

typedef struct {
    Range    ranges[MAX_ROWS];
    uint64_t ids[MAX_ROWS];
    int      rcount;
    int      icount;
} Database;

void database_free(Database* db);

void die(Database* db, char* message)
{
    database_free(db);
    if (errno) {
        perror(message);
    }
    else {
        printf("ERROR: %s\n", message);
    }
}

void database_fill(Database* db)
{
    FILE* fh = fopen(INPUT_FILE, "r");

    if (!fh) {
        die(db, "could not open file");
    }

    char*   line = NULL;
    size_t  len = 0;
    ssize_t read;

    int stage = 0;
    while ((read = getline(&line, &len, fh) != -1)) {

        if (line[0] == '\n') {
            stage++;
            continue;
        }

        line[strcspn(line, "\n")] = '\0';

        if (stage == 0) {
            uint64_t start = 0, end = 0;
            if (sscanf(line, "%llu-%llu", &start, &end) == 2) {
                Range rg = { .start = start, .end = end };
                db->ranges[db->rcount] = rg;
                db->rcount++;
            }
        }
        else if (stage == 1) {
            uint64_t id = strtoll(line, NULL, 10);
            db->ids[db->icount] = id;
            db->icount++;
        }
        else {
            die(db, "oops");
        }
    }

    fclose(fh);
    free(line);
}

void database_print(Database* db)
{
    for (int i = 0; i < db->rcount; i++) {
        printf("range %d:\n", i);
        printf("\tstart : %llu\n\tend   : %llu\n", db->ranges[i].start, db->ranges[i].end);
    }
    printf("\n");

    for (int i = 0; i < db->icount; i++) {
        printf("ID %d: %llu\n", i, db->ids[i]);
    }
    printf("\n");
}

void database_free(Database* db)
{
    if (db) {
        free(db);
    }
}

Database* database_init()
{
    Database* db = malloc(sizeof(Database));

    if (!db) {
        die(NULL, "Could not allocate memory for database\n");
    }

    db->rcount = 0;
    db->icount = 0;

    return db;
}

bool is_fresh(Database* db, uint64_t id)
{
    for (int i = 0; i < db->rcount; i++) {
        if (id >= db->ranges[i].start && id <= db->ranges[i].end) {
            return true;
        }
    }
    return false;
}

int main(int argc, char* argv[])
{
    Database* db = database_init();
    database_fill(db);

    // database_print(db);

    int fresh_c = 0;
    for (int i = 0; i < db->icount; i++) {
        if (is_fresh(db, db->ids[i])) {
            fresh_c++;
        }
    }
    printf("%d fresh ingredients\n", fresh_c);

    database_free(db);
    return 0;
}
