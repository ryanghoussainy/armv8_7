#include "parse-asm.h"

void handle_aliases(Instruction* instr) {
    char* rzr_str = instr->o1.reg[0] == 'x' ? "xzr" : "wzr";

    union Operand rzr;
    strcpy(rzr.reg, rzr_str);

    char* op = instr->operation;
    if (strcmp(op, "cmp") == 0 || strcmp(op, "cmn") == 0 || strcmp(op, "tst") == 0) {
        if (strcmp(op, "cmp") == 0) {
            strcpy(op, "subs");
        } else if (strcmp(op, "cmn") == 0) {
            strcpy(op, "adds");
        } else if (strcmp(op, "tst") == 0) {
            strcpy(op, "ands");
        }

        instr->o3 = instr->o2;
        instr->o3_type = instr->o2_type;

        instr->o2 = instr->o1;
        instr->o2_type = instr->o1_type;

        instr->o1 = rzr;
        instr->o1_type = REGISTER;
    } else if (strstr(op, "neg") != NULL || strcmp(op, "mvn") == 0 || strcmp(op, "mov") == 0) {
        if (strcmp(op, "neg") == 0) {
            strcpy(op, "sub");
        } else if (strcmp(op, "negs") == 0) {
            strcpy(op, "subs");
        } else if (strcmp(op, "mvn") == 0) {
            strcpy(op, "orn");
        } else if (strcmp(op, "mov") == 0) {
            strcpy(op, "orr");
        }

        instr->o3 = instr->o2;
        instr->o3_type = instr->o2_type;

        instr->o2 = rzr;
        instr->o2_type = REGISTER;
    } else if (strcmp(op, "mul") == 0 || strcmp(op, "mneg") == 0) {
        if (strcmp(op, "mul") == 0) {
            strcpy(op, "madd");
        } else if (strcmp(op, "mneg") == 0) {
            strcpy(op, "msub");
        }

        instr->o4 = rzr;
        instr->o4_type = REGISTER;
    }
}

uint64_t string_to_int(char* str) {
    // turns a string into int, handles both denary and hexadecimal
    if (strlen(str) == 1) {
        return atoi(str);
    } else if (strncmp(str, "0x", 2) == 0) {
        // hexadecimal
        return strtol(str, NULL, 16);
    } else {
        return atoi(str);
    }
}


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


enum OPERAND_TYPE extract_shift_type(char* str) {
    size_t word_count;
    char* str_copy = malloc(sizeof(str));
    strcpy(str_copy, str);
    char** operand = split_string(str_copy, " ", &word_count);
    if (word_count != 2) {
        // not a shift
        return NONE;
    }
    if (strcmp(operand[0], "lsl") == 0) {
        return LSL;
    } else if (strcmp(operand[0], "lsr") == 0) {
        return LSR;
    } else if (strcmp(operand[0], "asr") == 0) {
        return ASR;
    } else if (strcmp(operand[0], "ror") == 0) {
        return ROR;
    } else {
        // not a shift
        return NONE;
    }
}

int extract_shift_bits(char* str) {
    size_t word_count;
    char* str_copy = malloc(sizeof(str));
    strcpy(str_copy, str);
    char** operand = split_string(str_copy, " ", &word_count);
    if (word_count != 2) {
        // error
        return -1;
    }
    return string_to_int(operand[1] + 1);
}


uint64_t register_number(const char* str, bool* is_64_bit) {
    // An example of how to call the function

    /*
        char example[] = "x13";
        bool is_64_bit;
        int example_reg = register_number(example, &is_64_bit);
        printf("%lu", example_reg); // 13
        printf("%d", is_64_bit); // 1 (true)
    */

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

enum INSTRUCTION_TYPE classify_instruction(char* operation)  {
    if (strcmp(operation, "b") == 0 || strncmp(operation, "b.", 2) == 0) {
        return BRANCH;
    } else if (strcmp(operation, "ldr") == 0 || strcmp(operation, "str") == 0) {
        return TRANSFER;
    } else {
        return DP;
    }
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
    } else if (extract_shift_type(operand) != NONE) {
        return extract_shift_type(operand);
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
            new_operand.number = string_to_int(str + 1) - address;
            break;
        case REGISTER:
            strcpy(new_operand.reg, str);
            break;
        case ADDRESS:
            new_operand.number = get_value(map, str) - address;
            break;  
        case LSL:
        case LSR:
        case ASR:
        case ROR:
            new_operand.number = string_to_int(str + 1);
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

    if (ins == NULL) {
        printf("Invalid instruction: %s", str);
        return new_ins;
    }

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


    // TODO: Separate handling for load and store as syntax is different

    if (classify_instruction(new_ins.operation) == TRANSFER) {
        // case 1: load literal

        // case 2: pre-index

        // case 3: post-index

        // case 4: unsigned offset

        // case 5: register offset
    } else {

        // fall-through switch statements
        switch (operand_count) {
            case 4:
                new_ins.o4_type = convert_address_to_literal(classify_operand((operands[3])));
                new_ins.o4 = build_operand(operands[3], map, address);
            case 3:
                new_ins.o3 = build_operand(operands[2], map, address);
                new_ins.o3_type = convert_address_to_literal(classify_operand((operands[2])));
            case 2:
                new_ins.o2_type = convert_address_to_literal(classify_operand((operands[1])));
                new_ins.o2 = build_operand(operands[1], map, address);
            case 1:
                new_ins.o1_type = convert_address_to_literal(classify_operand((operands[0])));
                new_ins.o1 = build_operand(operands[0], map, address);
                break;
            default:
                // error
                free(str_copy);

                // TODO: free the 2D arrays
                free(ins);
                free(operands);
                return new_ins;
        }

        handle_aliases(&new_ins);
    }

    free(str_copy);

    // TODO: free the 2D arrays
    free(ins);
    free(operands);

    return new_ins;
}

char* print_operand_type(enum OPERAND_TYPE op_type) {
    switch(op_type) {
        case REGISTER:
            return "REGISTER";
        case LITERAL:
            return "LITERAL";
        case LSL:
            return "LSL";
        case LSR:
            return "LSR";
        case ASR:
            return "ASR";
        case ROR:
            return "ROR";
        case ADDRESS:
            return "ADDRESS";
        case NONE:
            return "NONE";
        default:
            return "";
    }
}

void print_instruction(Instruction* instr) {
    printf("operation: %s\n", instr->operation);
    printf("o1.reg: %s\n", instr->o1.reg);
    printf("o1.number: %d\n", instr->o1.number);
    printf("o1_type: %s\n", print_operand_type(instr->o1_type));
    printf("o2.reg: %s\n", instr->o2.reg);
    printf("o2.number: %d\n", instr->o2.number);
    printf("o2_type: %s\n", print_operand_type(instr->o2_type));
    printf("o3.reg: %s\n", instr->o3.reg);
    printf("o3.number: %d\n", instr->o3.number);
    printf("o3_type: %s\n", print_operand_type(instr->o3_type));
    printf("o4.reg: %s\n", instr->o4.reg);
    printf("o4.number: %d\n", instr->o4.number);
    printf("o4_type: %s\n", print_operand_type(instr->o4_type));
}
