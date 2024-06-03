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


int pass_two(char* instructions[], Entry* map, size_t size) {

}


int main(int argc, char **argv) {
  return EXIT_SUCCESS;
}
