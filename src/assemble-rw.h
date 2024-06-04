#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#define MAX_SIZE 100

size_t read_asm(char* path, char*** all_lines);
int write_bin(char* path, uint32_t* words, size_t size);
