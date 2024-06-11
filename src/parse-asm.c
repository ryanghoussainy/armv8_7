#include "parse-asm.h"

/*
uint64_t val1 = string_to_int("0x1A"); // val1 = 26
uint64_t val2 = string_to_int("26"); // val2 = 26

Converts a string to an integer, handling both decimal and hexadecimal.
*/
uint64_t string_to_int(char* str) {
    if (strlen(str) == 1) {
        return atoi(str);
    } else if (strncmp(str, "0x", 2) == 0) {
        // hexadecimal
        return strtol(str, NULL, HEX_SIZE);
    } else {
        return atoi(str);
    }
}

/*
char* str = "  Hello";
remove_leading_spaces(str); // str = "Hello"

Removes leading spaces from a string.
*/
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

/*
char* str = "Hello  ";
remove_trailing_spaces(str); // str = "Hello"

Removes trailing spaces from a string.
*/
static void remove_trailing_spaces(char* str) {
    int len = strlen(str);
    int i;

    for (i = len - 1; i >= 0; i--) {
        if (!isspace((unsigned char)str[i])) {
            break;
        }
    }
    
    str[i + 1] = '\0'; 
}

/*
const char* str = "Hello";
char c = last_character(str);

Gets the last character from a string, assumes str is not empty
*/
static char last_character(const char* str) {
    const char* ptr = str;
    while (*ptr != '\0') {
        ptr++;
    }
    return *(ptr - 1);
}

/*
char* str = "Hello";
remove_last_character(str);

Removes the last character from a string
*/
void remove_last_character(char* str) {
    if (strlen(str) > 0) {
        str[strlen(str) - 1] = '\0';
    }
}

/*
size_t word_count;
char* str = "Hello World";
char** result = split_string(str, " ", &word_count);

Splits the string 'str' into an array of strings, using 'sep' as the delimiter and
keeping track of the word count.
*/
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

/*
char* shift_str = "lsl #2";
enum OPERAND_TYPE type = extract_shift_type(shift_str); // type = LSL

Extracts the shift type from a string, returning NONE if it is not a shift.
*/
enum OPERAND_TYPE extract_shift_type(char* str) {
    size_t word_count;
    char* str_copy = (char*) malloc(strlen(str) + 1);
    strcpy(str_copy, str);

    char** operand = split_string(str_copy, " ", &word_count);
    free(str_copy);

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

/*
char* shift_str = "lsl #2";
int shift = extract_shift_bits(shift_str); // shift = 2

Extracts the shift bits from a string, returning -1 if it is invalid.
*/
int extract_shift_bits(char* str) {
    size_t word_count;
    char* str_copy = (char*) malloc(strlen(str) + 1);
    strcpy(str_copy, str);
    
    char** operand = split_string(str_copy, " ", &word_count);
    free(str_copy);

    if (word_count != 2) {
        // error
        return -1;
    }
    return string_to_int(operand[1] + 1);
}

/*
char example[] = "x13";
bool is_64_bit;
int example_reg = register_number(example, &is_64_bit); // example_reg = 13, is_64_bit = true

Converts a string to a register number, setting is_64_bit to true if and only if
it is a 64-bit register.
*/
uint64_t register_number(const char* str, bool* is_64_bit) {
    if (strcmp(str, "xzr") == 0) {
        *is_64_bit = true;
        // ZR set to 31
        return ZR;
    } else if (strcmp(str, "wzr") == 0) {
        *is_64_bit = false;
        // ZR set to 31
        return ZR;
    } else if (strcmp(str, "PC") == 0) {
        *is_64_bit = true;
        // PC set to 32
        return PC;
    }

    *is_64_bit = str[0] == 'x';
    return atoi(str + 1);
}

/*
char str[] = "and x1, x2, x3";
enum LINE_TYPE type = classify_line(str); // type = INSTRUCTION

Classifies a line of assembly code as a directive, label, or instruction.
*/
enum LINE_TYPE classify_line(char str[]) {
    remove_trailing_spaces(str);
    if (str[0] == '.') {
        return DIRECTIVE;
    } else if (last_character(str) == ':') {
        return LABEL;
    } else {
        return INSTRUCTION;
    }
}

/*
char* operation = "cmp";
enum INSTRUCTION_TYPE type = classify_instruction(operation); // type = DP

Classifies an instruction as a branch, data processing, or transfer.
*/
enum INSTRUCTION_TYPE classify_instruction(char* operation)  {
    if (strcmp(operation, "b") == 0 || strncmp(operation, "b.", 2) == 0 || strcmp(operation, "br") == 0) {
        return BRANCH;
    } else if (strcmp(operation, "ldr") == 0 || strcmp(operation, "str") == 0) {
        return TRANSFER;
    } else {
        return DP;
    }
}

/*
char* str = "w1";
bool is_register = is_register(str); // is_register = true

Checks if a string is a valid register.
*/
bool is_register(char* str) {
    size_t len = strlen(str);
    if (len != 2 && len != 3) {
        return false;
    } else if(str[0] != 'w' && str[0] != 'x') {
        return false;
    }
    int reg_no = atoi(str + 1);
    return reg_no >= 0 && reg_no < ZR;
}

/*
char* str = "x1";
enum OPERAND_TYPE type = classify_operand(str); // type = REGISTER

Classifies an operand as a register, literal, shift, or address.
*/
enum OPERAND_TYPE classify_operand(char* operand) {
    enum OPERAND_TYPE type = extract_shift_type(operand);
    if (operand[0] == '#') {
        return LITERAL;
    } else if(is_register(operand)) {
        return REGISTER;
    } else if (type != NONE) {
        return type;
    } else {
        return ADDRESS;
    }
}

/*
enum OPERAND_TYPE type = ADDRESS;
type = convert_address_to_literal(type); // type = LITERAL

Converts an address to a literal.
*/
enum OPERAND_TYPE convert_address_to_literal(enum OPERAND_TYPE type) {
    if (type == ADDRESS) {
        return LITERAL;
    } else return type;
}

/*
char* operand; // example: "x1", "#2", "lsl #2"
Entry* map;
uint64_t address;
int is_offset;
union Operand new_operand = build_operand(operand, map, address, is_offset);

Builds an operand from a string, using the symbol table to resolve addresses.
*/
static union Operand build_operand(char* str, Entry* map, uint64_t address, int is_offset) {
    union Operand new_operand;
    switch(classify_operand(str)){
        case LITERAL:
            new_operand.number = string_to_int(str + 1) - (address * is_offset);
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
            new_operand.number = string_to_int(str + 5);
            break;
        case NONE:
            // error, should not have been called
            break;      
    }
    return new_operand;
}

/*
Instruction* instr;
handle_aliases(instr); // modifies instr to handle aliases

Converts instructions which are aliases for other instructions.
*/
static void handle_aliases(Instruction* instr) {
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

        instr->o4 = instr->o3;
        instr->o4_type = instr->o3_type;

        instr->o3 = instr->o2;
        instr->o3_type = instr->o2_type;

        instr->o2 = instr->o1;
        instr->o2_type = instr->o1_type;

        instr->o1 = rzr;
        instr->o1_type = REGISTER;
    } else if (strcmp(op, "mul") == 0 || strcmp(op, "mneg") == 0) {
        if (strcmp(op, "mul") == 0) {
            strcpy(op, "madd");
        } else if (strcmp(op, "mneg") == 0) {
            strcpy(op, "msub");
        }

        instr->o4 = rzr;
        instr->o4_type = REGISTER;
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

        instr->o4 = instr->o3;
        instr->o4_type = instr->o3_type;
        
        instr->o3 = instr->o2;
        instr->o3_type = instr->o2_type;

        instr->o2 = rzr;
        instr->o2_type = REGISTER;
    } 
}

/*
char* str = "ldr x1, [x2, #4]";
Entry* map;
uint64_t address;
Instruction new_ins = build_instruction(str, map, address);

Builds an instruction from a string, using the symbol table to resolve addresses.
*/
Instruction build_instruction(char* str, Entry* map, uint64_t address) {

    Instruction new_ins;

    remove_trailing_spaces(str);

    // set default values to NONE
    new_ins.o1_type = NONE;
    new_ins.o2_type = NONE;
    new_ins.o3_type = NONE;
    new_ins.o4_type = NONE;

    char* str_copy = (char *) malloc(strlen(str) + 1);
    strcpy(str_copy, str);

    size_t word_count;
    char** ins = split_string(str_copy, " ", &word_count);
    free(str_copy);

    if (ins == NULL) {
        printf("Invalid instruction: %s", str);
        return new_ins;
    }

    strcpy(new_ins.operation, ins[0]);

    // get the rest of instruction
    size_t len = strlen(new_ins.operation);

    char* str_copy2 = (char *) malloc(strlen(str) + 1);
    strcpy(str_copy2, str);

    size_t operand_count;
    char** operands = split_string(str_copy2 + len + 1, ",", &operand_count);
    free(str_copy2);

    // clear white spaces in front of operand
    for (int i = 0; i < operand_count; i++) {
        remove_leading_spaces(operands[i]);
    }

    enum INSTRUCTION_TYPE in_type = classify_instruction(new_ins.operation);
    int is_offset = 0;

    switch (in_type)
    {
    case TRANSFER:
        is_offset = 1;
        break;
    case BRANCH:
        is_offset = 1;
        break;
    default:
        break;
    }

    // Separate handling for load and store as syntax is different
    if (in_type == TRANSFER) {

        union Operand o4;

        new_ins.o1_type = REGISTER;
        new_ins.o1 = build_operand(operands[0], map, address, is_offset);

        if (last_character(str) == ']' && operands[1][0] == '[') {
            // unsigned offset or register offset
            new_ins.o2_type = REGISTER;

            if (operand_count == 2) {
                // zero unsigned offset
                remove_last_character(operands[1]);
                // +1 to remove ']'
                new_ins.o2 = build_operand(operands[1] + 1, map, address, is_offset);
                o4.number = UNSIGNED_OFFSET;
            } else {
                new_ins.o2 = build_operand(operands[1] + 1, map, address, is_offset);
                remove_last_character(operands[2]);  // remove ']'

                if (is_register(operands[2])) {
                    // register offset
                    new_ins.o3_type = REGISTER;
                    new_ins.o3 = build_operand(operands[2], map, 0, is_offset);
                    o4.number = REGISTER_OFFSET;
                } else {
                    new_ins.o3_type = LITERAL;
                    new_ins.o3 = build_operand(operands[2], map, 0, is_offset);
                    o4.number = UNSIGNED_OFFSET;
                }
            }
        } else if (last_character(str) == '!') {
            // pre-index
            new_ins.o2_type = REGISTER;
            new_ins.o2 = build_operand(operands[1] + 1,map,0, is_offset);

            new_ins.o3_type = LITERAL;
            remove_last_character(operands[2]);  // remove '!'
            remove_last_character(operands[2]);  // remove ']' 
            new_ins.o3 = build_operand(operands[2], map, 0, is_offset);

            o4.number = PRE_INDEX;
        } else if (operand_count > 2 && operands[2][0] == '#' && last_character(operands[2]) != ']') {
            // post-index 
            new_ins.o2_type = REGISTER;
            remove_last_character(operands[1]); // remove ']'
            new_ins.o2 = build_operand(operands[1] + 1, map, 0, is_offset);

            new_ins.o3_type = LITERAL;
            new_ins.o3 = build_operand(operands[2], map, 0, is_offset) ;

            o4.number = POST_INDEX;
        } else if (classify_operand(operands[1]) == LITERAL || classify_operand(operands[1]) == ADDRESS)  {
            // load literal    
            new_ins.o2_type = LITERAL;
            new_ins.o2 = build_operand(operands[1], map, address, is_offset);
            o4.number = LOAD_LITERAL;
        } else {
            printf("Invalid instruction format for SDTI");
        }
        
        new_ins.o4_type = LITERAL;
        new_ins.o4 = o4;

    } else {
        // fall-through switch statements
        switch (operand_count) {
            case 4:
                new_ins.o4_type = convert_address_to_literal(classify_operand((operands[3])));
                new_ins.o4 = build_operand(operands[3], map, address, is_offset);
            case 3:
                new_ins.o3 = build_operand(operands[2], map, address, is_offset);
                new_ins.o3_type = convert_address_to_literal(classify_operand((operands[2])));
            case 2:
                new_ins.o2_type = convert_address_to_literal(classify_operand((operands[1])));
                new_ins.o2 = build_operand(operands[1], map, address, is_offset);
            case 1:
                new_ins.o1_type = convert_address_to_literal(classify_operand((operands[0])));
                new_ins.o1 = build_operand(operands[0], map, address, is_offset);
                break;
            default:

                // TODO: free the 2D arrays
                free(ins);
                free(operands);
                return new_ins;
        }

        handle_aliases(&new_ins);
    }

    // TODO: free the 2D arrays
    free(ins);
    free(operands);

    return new_ins;
}

/*
enum OPERAND_TYPE op_type = REGISTER;
char* str = print_operand_type(op_type); // str = "REGISTER"

Returns the operand type as a string.
*/
static char* print_operand_type(enum OPERAND_TYPE op_type) {
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
