#include "loader.h"


static long get_file_size(FILE *fptr) {
  fseek(fptr, 0L, SEEK_END);
  long file_size = ftell(fptr);
  rewind(fptr);
  return file_size;
}

int load_binary(char *file_name, uint8_t *memory) {
  FILE *fptr;
  fptr = fopen(file_name, "rb");

  if (fptr == NULL) {
    printf("Unable to read file: %s\n", file_name);
    return 0;
  }
  
  long nbytes = get_file_size(fptr);
  
  for (int i = 0; i < nbytes; i++) {
    if (fread(memory + i, 1, 1, fptr) != 1) {
        printf("Error reading file: %s\n", file_name);
        return 0;
    }
  }
  fclose(fptr);
  return 1;
}
