#include <errno.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INPUT_FILE "input.txt"
#define MAX_ROWS 2048
#define MAX_DATA 4096

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

bool is_fresh_ingredient(Database* db, uint64_t id)
{
    for (int i = 0; i < db->rcount; i++) {
        if (id >= db->ranges[i].start && id <= db->ranges[i].end) {
            return true;
        }
    }
    return false;
}

int ranges_compare(const void* a, const void* b)
{
    Range ra = *(Range*)a;
    Range rb = *(Range*)b;

    if (ra.start < rb.start)
        return -1;

    if (ra.start > rb.start)
        return 1;

    return 0;
}

void database_merge_ranges(Database* db)
{
    // first we sort the ranges by start value
    qsort(db->ranges, db->rcount, sizeof(Range), ranges_compare);

    // start with first range
    int merged_idx = 0;

    // iterate over the rest
    for (int i = 1; i < db->rcount; i++) {
        Range* last = &db->ranges[merged_idx];
        Range* curr = &db->ranges[i];

        if (curr->start <= last->end) {
            // the current range overlap the last range added
            if (curr->end > last->end)
                // last range end value needs to be updated
                last->end = curr->end;
        }
        else {
            // no overlaps, just add the range
            merged_idx++;
            db->ranges[merged_idx] = *curr;
        }
    }

    // update rcount
    db->rcount = merged_idx + 1;
}

uint64_t total_fresh_ingredients(Database* db)
{
    uint64_t res = 0;

    for (int i = 0; i < db->rcount; i++) {
        Range    rg = db->ranges[i];
        uint64_t diff = rg.end - rg.start;
        res += diff + 1;
    }

    return res;
}

int main(int argc, char* argv[])
{
    Database* db = database_init();

    database_fill(db);
    database_merge_ranges(db);

    uint64_t total_fresh = total_fresh_ingredients(db);
    printf("%llu fresh ingredients in total\n", total_fresh);

    database_free(db);
    return 0;
}
