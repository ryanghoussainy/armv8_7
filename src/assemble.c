#include <stdlib.h>
#include "assemble-rw.h"
#include "directives.h"
#include "branch-asm.h"
#include "transfer-assemble.h"
#include "dp-assemble.h"

static size_t pass_one(char* instructions[], Entry* map, size_t size) {
  size_t count = 0;
  for (int line = 0; line < size; line++) {
    char *ins = strdup(instructions[line]);
    if (classify_line(ins) == LABEL) {
      remove_last_character(ins);
      if (add_entry(map, ins, line - count) == 0) {
        // error handling
        return 0;
      }
      count++;
    }
  }
  return count;
}

static int pass_two(char* instructions[], Entry* map, uint32_t* output, size_t size)  {
  size_t label_count = 0;
  for (int line = 0; line < size; line++) {
    uint32_t word = 0;
   
    switch(classify_line(instructions[line])) {
      Instruction ins;
      case INSTRUCTION:

        // vv DEBUGGING
        printf("Instruction: %s\n", instructions[line]);
        /*
        if (map != NULL) {
          print_all_entries(map);
        }
        printf("size: %zu\n", size);
        */
        // ^^ DEBUGGING

        ins = build_instruction(instructions[line], map, line - label_count);

        // TODO: Classify type of instruction then pass in respective functions
        switch(classify_instruction(ins.operation)) {
          case DP:
            // call DP function
            word = dp_assembly(&ins);
            break;
          case TRANSFER:
            // call transfer function
            word = transfer_assembly(&ins);
            break;
          case BRANCH:
            word = branch_assembly(&ins);
            break;
        }
        break;
      case DIRECTIVE:
        word = directive_binary(instructions[line]);
        break;
      case LABEL:
        label_count++;
        continue;
        break;
    }
    output[line-label_count] = word;
  }
  return 1;
}


int main(int argc, char **argv) {

  if (argc < 3) {
    // error
    printf("Need file in and file out arguments");
    return EXIT_FAILURE;
  } 

  // read from file, store into array of strings
  char** all_lines;
  size_t file_size = read_asm(argv[1], &all_lines);

  // pass one, store symbol table in map
  Entry map = {NULL, 0, NULL};
  size_t label_count = pass_one(all_lines, &map, file_size);

  // pass two, store result in array of uint32_t
  uint32_t* output = malloc(sizeof(uint32_t) * (file_size - label_count));
  pass_two(all_lines, map.next, output, file_size);

  // write array of uint32_t into bin file
  write_bin(argv[2], output, file_size - label_count);
  
  return EXIT_SUCCESS;
}
