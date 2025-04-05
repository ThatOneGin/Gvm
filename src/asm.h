#ifndef ASM_H
#define ASM_H
#include "gvm.h"
#include "instruction.h"
#include "string_view.h"
#include <stdlib.h>

typedef struct {
  unsigned char magic[3]; /* G V M, all in capital */
  size_t ps;
  int main_f;
} GHeader;

typedef struct {
  inst p[maxprogramcap];
  size_t ps;
  GHeader h;
} Gassembly;

Gassembly new_ga(void);
inst parse_line(String_view line);
void parse_file(String_view src, Gassembly *ga);
#endif
