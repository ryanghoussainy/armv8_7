#include "loader.h"


long getFileSize(FILE *fptr) {
  fseek(fptr, 0L, SEEK_END);
  long fileSize = ftell(fptr);
  rewind(fptr);
  return fileSize;
}

int loadBinary(char *file_name, uint8_t *memory) {
  FILE *fptr;
  fptr = fopen(file_name, "rb");

  if (fptr == NULL) {
    printf("Unable to read file: %s\n", file_name);
    return 0;
  }
  
  long nbytes = getFileSize(fptr);
  
  for (int i = 0; i < nbytes; i++) {
    if (fread(memory + i, 1, 1, fptr) != 1) {
        printf("Error reading file: %s\n", file_name);
        return 0;
    }
  }
  fclose(fptr);
  return 1;
}
