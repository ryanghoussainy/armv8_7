#pragma once
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "parse-asm.h"

#define MAX_SIZE 100

size_t read_asm(char* path, char*** all_lines);
void write_bin(char* path, uint32_t* words, size_t size);
