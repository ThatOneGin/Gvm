#include "asm.h"
#include "instruction.h"
#include "string_view.h"
#include <stdio.h>
#include <stdlib.h>

/* TODO: read binary files correctly */

String_view openfile(char *filepath) {
  FILE *f = fopen(filepath, "r");
  if (f == NULL) {
    perror("Could not read input file");
    exit(1);
  }
  
  if (fseek(f, 0, SEEK_END) < 0) {
    perror("Could not read input file");
    exit(1);
  }

  size_t sz = ftell(f);
  if (sz <= 0) {
    perror("Could not read input file");
    exit(1);
  }
  
  char *buf = malloc(sz);
  if (buf == NULL) {
    perror("Could not alloc memory for file");
    exit(1);
  }
  
  if (fseek(f, 0, SEEK_SET) < 0) {
    perror("Could not read input file");
    exit(1);
  }
  
  size_t flsz = fread(buf, 1, sz, f);
  if (ferror(f)) {
    perror("Could not read input file");
    exit(1);
  }
  fclose(f);
  return (String_view) {
    .size = flsz,
    .data = buf,
  };
}

void write_file(Gassembly ga, char *filename) {
  FILE *f;

  f = fopen(filename, "wb");

  if (!f) {
    perror("[gvm]: Cannot open output file");
    exit(1);
  }

  fwrite(&ga.h, sizeof(ga.h), 1, f);
  fwrite(ga.p, sizeof(inst), ga.h.ps, f);
  fclose(f);
}

Gassembly read_file(char *filename) {
  Gassembly ga;
  FILE *f;

  f = fopen(filename, "rb");

  if (!f) {
    perror("[gvm]: Cannot open input file");
    exit(1);
  }

  fread(&ga.h, sizeof(ga.h), 1, f);

  inst instr;
  for (size_t i  = 0; i < ga.h.ps; i++) {
    fread(&instr, sizeof(inst), 1, f);
    ga.p[i] = instr;
    instr = (inst){0};
  }

  fclose(f);

  return ga;
}
