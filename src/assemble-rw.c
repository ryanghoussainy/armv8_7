#include "assemble-rw.h"


size_t read_asm(char* path, char*** all_lines) {
    /* read from .s file */
    FILE *fptr;
    fptr = fopen(path, "r");
    if (fptr == NULL) {
        printf("Failed to open file");
        return -1;
    }

    // set to 100 characters per line

    int counter = 0;
    *all_lines = malloc(sizeof(char*));
    (*all_lines)[0] = malloc(sizeof(char) * MAX_SIZE);

    char this_line[MAX_SIZE];

    while (fgets(this_line, MAX_SIZE, fptr)) {
        remove_leading_spaces(this_line);
        strcpy((*all_lines)[counter], this_line);
        counter++;
        *all_lines = realloc(*all_lines, sizeof(char*) * (counter + 1));
        (*all_lines)[counter] = malloc(sizeof(char) * MAX_SIZE);
    }

    fclose(fptr);
    return counter;
}


int write_bin() {
    /* write to a binary file */
}
