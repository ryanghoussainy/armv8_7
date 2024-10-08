#include "assemble-rw.h"


size_t read_asm(char* path, char*** all_lines) {
    /* read from .s file */
    FILE *fptr;
    fptr = fopen(path, "r");
    if (fptr == NULL) {
        printf("Failed to open file");
        fclose(fptr);
        exit(1);
    }

    // set to 100 characters per line

    int counter = 0;
    *all_lines = malloc(sizeof(char*));
    assert(*all_lines != NULL);

    (*all_lines)[0] = malloc(sizeof(char) * MAX_SIZE);
    assert((*all_lines)[0] != NULL);

    char this_line[MAX_SIZE];

    while (fgets(this_line, MAX_SIZE, fptr)) {
        remove_leading_spaces(this_line);
        remove_last_character(this_line);  // get rid of newline character
        if (strcmp(this_line, "")) {
            strcpy((*all_lines)[counter], this_line);
            counter++;
            *all_lines = realloc(*all_lines, sizeof(char*) * (counter + 1));
            assert(*all_lines != NULL);

            (*all_lines)[counter] = malloc(sizeof(char) * MAX_SIZE);
            assert((*all_lines)[counter] != NULL);
        }
    }

    free((*all_lines)[counter]);  // free the last malloc
    *all_lines = realloc(*all_lines, sizeof(char*) * counter);
    assert(*all_lines != NULL);

    fclose(fptr);
    return counter;
}


void write_bin(char* path, uint32_t* words, size_t size){
    FILE *fptr = fopen(path, "wb");
    size_t written = fwrite(words, sizeof(uint32_t), size, fptr);

    if(written != size) {
        // error
        fclose(fptr);
        exit(1);
    }

    fclose(fptr);
}
