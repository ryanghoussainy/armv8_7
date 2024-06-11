#include "symbol-table.h"

Entry* last_entry(Entry* map) {
    // assume map != NULL
    Entry* current = map;
    while (current->next != NULL) {
        current = current->next;
    }
    return current;
}


int add_entry(Entry* map, char* label, int value) {
    Entry* end = last_entry(map);
    Entry* new_entry = malloc(sizeof(Entry));
    assert(new_entry != NULL);

    new_entry->label = strdup(label); // Duplicate the label string
    assert(new_entry->label != NULL);
    
    new_entry->value = value;
    new_entry->next = NULL;
    end->next = new_entry;

    return 1;
}


int get_value(Entry* map, char* label) {
    while (map != NULL) {
        if (strcmp(map->label, label) == 0) {
            return map->value;
        } else {
            map = map->next;
        }
    }
    // not found
    return -1;
}

void print_entry(Entry* map) {
    if (map != NULL) {
        printf("label: %s\n", map->label);
        printf("value: %d\n", map->value);
    } else {
        printf("Invalid input - input is NULL\n");
    }
}

void print_all_entries(Entry* map) {
    if (map != NULL) {
        print_entry(map);
        Entry* next = map->next;
        while (next != NULL) {
            print_entry(next);
            next = next->next;
        }
    } else {
        printf("Invalid input - input is NULL\n");
    }
}

void free_entries(Entry* map) {
    if (map == NULL) return;
    if (map->next != NULL) free_entries(map->next);
    free(map->label);
    free(map);
}
