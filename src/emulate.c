#include <stdlib.h>
#include <stdio.h>
#include <string.h>

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
  return EXIT_SUCCESS;
}
