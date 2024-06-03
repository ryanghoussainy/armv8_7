#pragma once
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

long get_file_size(FILE *fptr);
int load_binary(char *file_name, uint8_t *memory);
