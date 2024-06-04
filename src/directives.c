#include "directives.h"


uint32_t directive_binary(char* str) {
    size_t word_count;
    char** ins = split_string(str, " ", &word_count);
    if (word_count != 2) {
        // error
        return 0;
    }
    return string_to_int(ins[1]);
}   
