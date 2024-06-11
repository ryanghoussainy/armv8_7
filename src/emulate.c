#include <string.h>
#include <assert.h>
#include "loader.h"
#include "execute.h"

int main(int argc, char **argv) {

  char *FILE_IN = malloc(20 * sizeof(char));
  assert(FILE_IN != NULL);
  char *FILE_OUT = malloc(20 * sizeof(char));
  assert(FILE_OUT != NULL);

  // Need to dynamically allocate memory for large filenames

  if (argc == 2 || argc == 3) {

    if (strlen(argv[1]) >= (20 * sizeof(char))) {

      // If the filename is larger than 20 characters then reallocate to accommodate

      FILE_IN = (char *) realloc(FILE_IN, (strlen(argv[1]) + 1) * sizeof(char));
      assert(FILE_IN != NULL);

    }

    strcpy(FILE_IN, argv[1]);
    printf("Input file is %s\n", FILE_IN);

    if (argc == 3) {

      if (strlen(argv[2]) >= (20 * sizeof(char))) {

        FILE_OUT = (char *) realloc(FILE_OUT, (strlen(argv[2]) + 1) * sizeof(char));
        assert(FILE_OUT != NULL);
      }

      strcpy(FILE_OUT, argv[2]);
      printf("Output file is %s\n", FILE_OUT);

    } else {

      strcpy(FILE_OUT, "stdout");

    }

  } else {

    printf("Wrong number of arguments supplied\n");
    free(FILE_IN);
    free(FILE_OUT);
    return EXIT_FAILURE;

  }

  
  CPU cpu;

  initialise_cpu(&cpu);

  // Load binary file into memory
  if (!load_binary(FILE_IN, cpu.memory)) {
    free(FILE_IN);
    free(FILE_OUT);
    return EXIT_FAILURE;
  }

  cycle(&cpu);

  if (strcmp(FILE_OUT, "stdout") == 0) {

    print_cpu(&cpu, stdout);

  } else {

    FILE* out = fopen(FILE_OUT, "w");

    print_cpu(&cpu, out);

  }

  free(FILE_IN);
  free(FILE_OUT);

  return EXIT_SUCCESS;
}
