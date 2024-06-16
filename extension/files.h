#pragma once
#include <stdlib.h>
#include <assert.h>
#include "linked-list.h"


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
    LinkedList* files;
    LinkedList* directories;
    char *path;
    int path_size;
} Directory;
