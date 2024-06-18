#include "instruction.h"

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
    assert(token != NULL);

    while (token != NULL) {
        words = realloc(words, sizeof(char*) * (*word_count + 1));
        assert(words != NULL);

        words[*word_count] = malloc(strlen(token) + 1);
        assert(words[*word_count] != NULL);

        strcpy(words[*word_count], token);
        token = strtok(NULL, sep);  // get next token
        (*word_count)++;
    }
    
    return words;
}

/*
char* operation = malloc(50);
assert(operation != NULL);
strcpy(operation, "ls");
Operation op = parse_to_operation(operation);

Converts a string input into its Operation enum type equivalent
*/
enum Operation parse_to_operation(char* operation) {
    if (strcmp(operation, "ls") == 0){
        return LS;
    } else if (strcmp(operation, "man") == 0){
        return MAN;
    } else if (strcmp(operation, "cd") == 0){
        return CD;
    } else if (strcmp(operation, "touch") == 0) {
        return TOUCH;
    } else if (strcmp(operation, "mkdir") == 0) {
        return MKDIR;
    } else {
        return NONE;
    }
}

char* get_manual(enum Operation op) {
    switch (op) {
        case LS:
            return "ls manual goes here";
            break;
        case MAN:
            return "man manual goes here";
            break;
        case CD:
            return "cd manual goes here";
            break;
        default:
            return "no manual";
            break;
    }
}

/*
char* instruction = malloc(50);
assert(instruction != NULL);
strcpy(instruction, "ls -l");
Instruction instr = parse_to_instruction(instruction);

Parses a string input (assuming it is well formed) to a useable Instruction type equivalent
*/
Instruction parse_to_instruction(char* instruction) {
    Instruction ins;
    size_t word_count;

    enum Operation operation;
    char* options;
    char** arguments;
    size_t argument_count = 0;
    char* manual;

    bool has_options = false;
    char** split_instruction = split_string(instruction, " ", &word_count);

    if (word_count >= 1) {
        operation = parse_to_operation(split_instruction[0]);
        manual = get_manual(operation);
    }

    if (word_count >= 2 && strcmp((char[2]) {split_instruction[1][0], '\0'}, "-") == 0) {
        options = malloc(strlen(split_instruction[1] - 1));
        assert(options != NULL);
        options = split_instruction[1] + 1;
        has_options = true;
    }

    if (word_count >= 2) {
        argument_count = has_options ? word_count - 2 : word_count - 1;
        int start_pointer = has_options ? 2 : 1;
        arguments = malloc(sizeof(char*) * argument_count);
        assert(arguments != NULL);

        for (int i = 0; i < word_count - start_pointer; i++) {
            arguments[i] = malloc(strlen(split_instruction[i + start_pointer]) + 1);
            assert(arguments[i] != NULL);
            arguments[i] = split_instruction[i + start_pointer];
        }
    }

    ins.operation = operation;
    ins.options = options;
    ins.arguments = arguments;
    ins.argument_count = argument_count;
    ins.manual = manual;

    return ins;
}

/*
char* instruction = malloc(50);
assert(instruction != NULL);
strcpy(instruction, "ls -l");
Instruction instr = parse_to_instruction(instruction);
free_instruction(instr);

Frees the memory of an Instruction struct
*/
int free_instruction(Instruction* instruction) {
    free(instruction);
    return 1;
}

void output_instruction(Instruction* instruction) {
    printf("options: %s\n", instruction->options);
    printf("arguments: ");
    for (int i = 0; i < instruction->argument_count; i++) {
        printf(" %s ", instruction->arguments[i]);
    }
    printf("\nmanual: %s\n", instruction->manual);
}

/*
int main(int argc, char **argv) {
    char* instruction_string = malloc(20);
    strcpy(instruction_string, "ls -l arg1 arg2");
    Instruction* instruction = malloc(sizeof(Instruction));
    *instruction = parse_to_instruction(instruction_string);
    free_instruction(instruction);
}
*/