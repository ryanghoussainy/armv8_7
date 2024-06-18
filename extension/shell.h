#pragma once
#include "files.h"


typedef struct {
    Directory* root;
    Directory* current_directory;
    char* path;
    FILE* out;
} Shell;

void initialise_shell(Shell* shell, FILE* out);
void print_shell(Shell* shell);
