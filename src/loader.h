#pragma once
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

long getFileSize(FILE *fptr);
int loadBinary(char fileName[20], uint8_t *memory);
