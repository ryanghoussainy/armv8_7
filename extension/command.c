#include "command.h"

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
char* command = malloc(50);
assert(command != NULL);
strcpy(command, "ls -l");
Command cmd = parse_to_command(command);

Parses a string input (assuming it is well formed) to a useable Command type equivalent
*/
Command parse_to_command(char* command) {
    Command cmd;
    size_t word_count;

    enum Operation operation;
    char* options;
    char** arguments;
    size_t argument_count = 0;
    char* manual;

    bool has_options = false;
    char** split_command = split_string(command, " ", &word_count);

    if (word_count >= 1) {
        operation = parse_to_operation(split_command[0]);
        manual = get_manual(operation);
    }

    if (word_count >= 2 && strcmp((char[2]) {split_command[1][0], '\0'}, "-") == 0) {
        options = malloc(strlen(split_command[1] - 1));
        assert(options != NULL);
        options = split_command[1] + 1;
        has_options = true;
    }

    if (word_count >= 2) {
        argument_count = has_options ? word_count - 2 : word_count - 1;
        int start_pointer = has_options ? 2 : 1;
        arguments = malloc(sizeof(char*) * argument_count);
        assert(arguments != NULL);

        for (int i = start_pointer; i < word_count; i++) {
            arguments[i] = malloc(strlen(split_command[i]) + 1);
            assert(arguments[i] != NULL);
            arguments[i] = split_command[i];
        }
    }

    cmd.operation = operation;
    cmd.options = options;
    cmd.arguments = arguments;
    cmd.argument_count = argument_count;
    cmd.manual = manual;

    return cmd;
}

/*
char* command = malloc(50);
assert(command != NULL);
strcpy(command, "ls -l");
Command cmd = parse_to_command(command);
free_command(cmd);

Frees the memory of an Command struct
*/
int free_command(Command* command) {
    free(command->options);
    for (int i = 0; i < command->argument_count; i++) {
        free(command->arguments[i]);
    }
    free(command->arguments);
    free(command->manual);
    return 1;
}