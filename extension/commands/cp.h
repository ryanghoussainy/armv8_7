#pragma once
#include "../shell.h"
#include "../files.h"


void cp(Shell* shell, char** srcs, int src_count, char* dest);
void cp_mv(Shell* shell, char** srcs, int src_count, char* dest, bool is_mv);
