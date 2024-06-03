#include "parse-asm.h"

void remove_leading_spaces(char* str) {
    int i = 0;
    int j = 0;

    // Skip leading white spaces
    while (str[i] != '\0' && isspace((unsigned char)str[i])) {
        i++;
    }

    // Shift the rest of the string to the front
    if (i > 0) {
        while (str[i] != '\0') {
            str[j++] = str[i++];
        }
        str[j] = '\0';
    } 
}


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
        // ZR set to 31
        return ZR;
    } else if (strcmp(str, "wzr")) {
        *is_64_bit = false;
        // ZR set to 31
        return ZR;
    } else if (strcmp(str, "PC")) {
        *is_64_bit = true;
        // PC set to 32
        return PC;
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
    // TODO: classify the different types of instructions
    return NULL;
}


bool is_register(char* str) {
    size_t len = strlen(str);
    if (len != 2 && len != 3) {
        return false;
    } else if(str[0] != 'w' && str[0] != 'x') {
        return false;
    } 
    return atoi(str + 1) >= 0 && atoi(str + 1) < 31;
}


enum OPERAND_TYPE classify_operand(char* operand) {
    // classify the different types of operands
    if (operand[0] == '#') {
        return LITERAL;
    } else if(is_regsiter(operand)) {
        return REGISTER;
    } else {
        return ADDRESS;
    }
    return NULL;
}


union Operand build_operand(char* str, Entry* map) {
    union Operand new_operand;
    switch(classify_instruction(str)){
        case LITERAL:
            new_operand.literal = atoi(str + 1);
            break;
        case REGISTER:
            strcpy(new_operand.reg, str);
            break;
        case ADDRESS:
            new_operand.literal = get_value(map, str);
            break;
    }
    return new_operand;
}


Instruction build_instruction(char* str, Entry* map) {
    Instruction new_str;

    /* TODO: Implement this  */

    return new_str;
}
