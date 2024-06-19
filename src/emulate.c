#include <string.h>
#include <assert.h>
#include "loader.h"
#include "execute.h"

int main(int argc, char **argv) {

  char* FILE_IN;
  char* FILE_OUT;

  if (argc == 2) {
    FILE_IN = argv[1];
    FILE_OUT = "stdout";
  } else if (argc == 3) {
    FILE_IN = argv[1];
    FILE_OUT = argv[2];
  } else {
    printf("Wrong number of arguments supplied\n");
    return EXIT_FAILURE;
  }
  
  CPU cpu;

  initialise_cpu(&cpu);

  // Load binary file into memory
  if (!load_binary(FILE_IN, cpu.memory)) {
    return EXIT_FAILURE;
  }

  cycle(&cpu);

  if (strcmp(FILE_OUT, "stdout") == 0) {

    print_cpu(&cpu, stdout);

  } else {

    FILE* out = fopen(FILE_OUT, "w");

    print_cpu(&cpu, out);

  }

  return EXIT_SUCCESS;
}
