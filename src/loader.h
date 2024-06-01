#pragma once
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

long getFileSize(FILE *fptr);
int loadBinary(char *file_name, uint8_t *memory);
