#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>
#include "linked-list.h"


typedef enum ELEMENT_TYPE {
    SFILE,
    SDIRECTORY,
} ELEMENT_TYPE;

struct File{
    char *name;
    char *content;
    char *path;
    struct Directory *parent;
};

typedef struct File File;

struct Directory {
    char *name;
    LinkedList* files;
    LinkedList* directories;
    char *path;
    struct Directory *parent;
};

typedef struct Directory Directory;

char* get_file_path(const char* dir, const char* name);
char* previous_directory_path(char* path);
ELEMENT_TYPE identify_type(void* item);
File* create_file(Directory* dir, char* name);
Directory* create_dir(Directory* dir, char* name);
void file_write(File* file, char* content, bool append);
int dir_remove_file(Directory* dir, File* file);
int dir_remove_directory(Directory* dir, Directory* rm_dir);
File* dir_find_file(Directory* dir, char* name);
Directory* dir_find_directory(Directory* dir, char* name);
File* copy_file(File* file);
Directory* copy_dir(Directory* dir);
void free_file(File* file);
void free_dir(Directory* dir);
