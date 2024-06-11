#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <assert.h>

typedef struct Entry {
    // linked-list style map from string to int
    char* label;
    int value;
    struct Entry* next;
} Entry;


int add_entry(Entry* map, char* label, int value);
int get_value(Entry* map, char* label);
void print_entry(Entry* map);
void print_all_entries(Entry* map);
void free_entries(Entry* map);
