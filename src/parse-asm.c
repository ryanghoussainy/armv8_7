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
    } else if(is_register(operand)) {
        return REGISTER;
    } else {
        return ADDRESS;
    }
}


enum OPERAND_TYPE convert_address_to_literal(enum OPERAND_TYPE type) {
    /* This function is needed as address is no longer needed when forming struct instruction */
    if (type == ADDRESS) {
        return LITERAL;
    } else return type;
}


union Operand build_operand(char* str, Entry* map, uint64_t address) {
    union Operand new_operand;
    switch(classify_operand(str)){
        case LITERAL:
            new_operand.literal = atoi(str + 1) - address;
            break;
        case REGISTER:
            strcpy(new_operand.reg, str);
            break;
        case ADDRESS:
            new_operand.literal = get_value(map, str) - address;
            break;      
        case NONE:
            // error, should not have been called
            break;      
    }
    return new_operand;
}


Instruction build_instruction(char* str, Entry* map, uint64_t address) {
    Instruction new_ins;

    char* str_copy = malloc(sizeof(str));
    strcpy(str_copy, str);

    size_t word_count;
    char** ins = split_string(str_copy, " ", &word_count);
    strcpy(new_ins.operation, ins[0]);

    // get the rest of instruction
    size_t len = strlen(new_ins.operation);

    // TODO: Handle case where there is no operand

    size_t operand_count;
    char** operands = split_string(str + len + 1, ",", &operand_count);

    // clear white spaces in front of operand
    for (int i = 0; i < operand_count; i++) {
        remove_leading_spaces(operands[i]);
    }


    // fall-through switch statements
    switch (operand_count) {
        case 4:
            new_ins.o4_type = convert_address_to_literal(classify_operand((operands[3])));
            new_ins.o4 = build_operand(operands[3], map, address);
        case 3:
            new_ins.o3_type = convert_address_to_literal(classify_operand((operands[2])));
            new_ins.o3 = build_operand(operands[2], map, address);
        case 2:
            new_ins.o2_type = convert_address_to_literal(classify_operand((operands[1])));
            new_ins.o2 = build_operand(operands[1], map, address);
        case 1:
            new_ins.o1_type = convert_address_to_literal(classify_operand((operands[0])));
            new_ins.o1 = build_operand(operands[0], map, address);
            break;
        default:
            // error
            return new_ins;
    }

    return new_ins;
}
