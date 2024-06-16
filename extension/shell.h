#pragma once
#include "files.h"


typedef struct {
    Directory* root;
    Directory* current_directory;
    char* path;
    int path_size;
} Shell;
