#include "instruction.h"
#include <stdio.h>
#include <stdlib.h>

void write_file(inst *p, size_t size, char *filename) {
  FILE *f;

  f = fopen(filename, "wb");

  if (!f) {
    perror("[gvm]: Cannot open output file");
    exit(1);
  }

  fwrite(p, sizeof(inst), size, f);
  fclose(f);
}

inst *read_file(char *filename) {
  FILE *f;

  f = fopen(filename, "rb");

  if (!f) {
    perror("[gvm]: Cannot open input file");
    exit(1);
  }

  fseek(f, 0L, SEEK_END);
  size_t c = ftell(f) / sizeof(inst);
  rewind(f);

  inst *program = malloc(sizeof(inst) * c);

  if (program == NULL) {
    perror("[gvm]: Cannot allocate memory for program. exiting");
    exit(1);
  }

  inst instruction;
  for (size_t i  = 0; i < c; i++) {
    fread(&instruction, sizeof(inst), 1, f);
    program[i] = instruction;
  }

  fclose(f);

  return program;
}
