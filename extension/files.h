#pragma once
#include <stdlib.h>
#include <assert.h>
#include "linked-list.h"


enum ELEMENT_TYPE {
    FILE,
    DIRECTORY,
};

typedef struct
{
    char *name;
    char name_size;
    char *content;
    int size;
    char *path;
    int path_size;
} File;

typedef struct
{
    char *name;
    char name_size;
    Node* files;
    Node* directories;
    char *path;
    int path_size;
} Directory;
