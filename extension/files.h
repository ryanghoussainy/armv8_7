#pragma once
#include "file-list.h"


typedef struct {
    char* name;
    FileList* files;
} File;

typedef struct {
    char* name;
    File* files;
    Directory* directories;
} Directory;
