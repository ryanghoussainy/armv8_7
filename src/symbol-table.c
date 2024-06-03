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
    new_entry->label = strdup(label); // Duplicate the label string
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
    return -1;
}
