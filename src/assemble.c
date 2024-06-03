#include <stdlib.h>
#include "assemble-rw.h"
#include "parse-asm.h"


int pass_one(char* instructions[], Entry* map, size_t size) {
  size_t count = 0;
  for (int line = 0; line < size; line++) {
    char *ins = strdup(instructions[line]);
    if (classify_line(ins) == LABEL) {
      remove_last_character(ins);
      if (add_entry(map, ins, line - count) == 0) {
        // error handeling
        return 0;
      }
      count++;
    }
  }
  return count;
}

int pass_two(char* instructions[], Entry* map, size_t size, uint32_t* output)  {
  output = malloc(sizeof(uint32_t) * size);
  int label_count = 0;
  for (int line = 0; line < size; line++) {
    switch(classify_line(instructions[line])) {
      case INSTRUCTION:
        break;
      case DIRECTIVE:
        break;
      case LABEL:
        label_count++;
        continue;
    }
  }
}


int main(int argc, char **argv) {

  // read from file, store into array of strings

  // pass one, store symbol table in map

  // pass two, store result in array of uint32_t

  // write array of uint32_t into bin file

  return EXIT_SUCCESS;
}
