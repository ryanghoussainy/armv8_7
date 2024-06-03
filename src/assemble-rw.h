#pragma once
#include <stdio.h>
#include <stdlib.h>
#include "parse-asm.h"

#define MAX_SIZE 100

size_t read_asm(char* path, char*** all_lines);
