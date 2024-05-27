#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "cpu.h"
#include <stdio.h>

int main(int argc, char **argv) {

  char FILE_IN[20] = {};
  char FILE_OUT[20] = "stdout";

  if (argc == 2) {
    strcpy(FILE_IN, argv[1]);
    printf("Input file is %s\n", FILE_IN);
    // Put code to run with 1 argument here
    
  } else if (argc == 3) {
    strcpy(FILE_IN, argv[1]);
    strcpy(FILE_OUT, argv[2]);
    printf("Input file is %s\n", FILE_IN);
    printf("Output file is %s\n", FILE_OUT);
    // Put code to run with 2 arguments here

  } else {
    printf("Wrong number of arguments supplied");
    return EXIT_FAILURE;
  }
  struct CPU cpu;

  initialise_cpu(&cpu);

  write_byte_memory(&cpu, 0, 1);
  write_byte_memory(&cpu, 1, 2);

  write_byte_memory(&cpu, 4, 4);
  write_byte_memory(&cpu, 5, 5);

  set_flag(&cpu, N, 1);

  print_cpu(&cpu, stdout);

  return EXIT_SUCCESS;
}
