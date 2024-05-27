#include <stdlib.h>
#include "cpu.h"
#include <stdio.h>

int main(int argc, char **argv) {
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
