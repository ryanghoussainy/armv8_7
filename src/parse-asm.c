#include "parse-asm.h"

char last_character(const char* str) {
    /* Gets the last character from a string, assumes str is not empty */
    const char* ptr = str;
    while (*ptr != '\0') {
        ptr++;
    }
    return *(ptr - 1);
}

void remove_last_character(char* str) {
    if (strlen(str) > 0) {
        str[strlen(str) - 1] = '\0';
    }
}

char** split_string(char str[], const char* sep, size_t* word_count) {
    *word_count = 0;
    char* token;
    char** words = NULL;

    token = strtok(str, sep);
    while (token != NULL) {
        words = realloc(words, sizeof(char*) * (*word_count + 1));
        words[*word_count] = malloc(strlen(token) + 1);
        strcpy(words[*word_count], token);
        token = strtok(NULL, sep);
        (*word_count)++;
    }

    return words;
}


uint64_t register_number(const char* str, bool* is_64_bit) {
    if (strcmp(str, "xzr")) {
        *is_64_bit = true;
        return ZR;
    } else if (strcmp(str, "wzr")) {
        *is_64_bit = false;
        return ZR;
    }

    *is_64_bit = str[0] == 'x';
    return atoi(str + 1);
}

enum LINE_TYPE classify_line(char str[]) {
    if (str[0] == '.') {
        return DIRECTIVE;
    } else if (last_character(str) == ':') {
        return LABEL;
    } else {
        return INSTRUCTION;
    }
}


enum INSTRUCTION_TYPE classify_instruction(Instruction* ins)  {
    // classify the different types of instructions
    return NULL;
}
