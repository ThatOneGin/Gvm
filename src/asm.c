#include "asm.h"
#include "gvm.h"
#include "instruction.h"
#include "string_view.h"
#include <stdlib.h>
#include <stdio.h>

Gassembly new_ga(void) {
  return (Gassembly){
    .h.magic = "GVM",
    .ps = 0,
    .h.ps = 0,
  };
}

inst parse_line(String_view line) {
  line = str_triml(line);
  String_view in = str_split(&line, ' ');

  if (str_cmp(in, to_str("push"))) {
    line = str_triml(line);
    int op = str_atoi(str_triml(line));
    return (inst){.kind=push, .op=op};
  } else if (str_cmp(in, to_str("shout"))) {
    line = str_triml(line);
    return (inst){.kind=shout, .op=0};
  } else if (str_cmp(in, to_str("halt"))) {
    return (inst){.kind=halt, .op=0};
  } else {
    printf("[Error]: opcode not recognized `%.*s`", (int)in.size, in.data);
    exit(1);
  }
}

void parse_file(String_view src, Gassembly *ga) {
  while (src.size > 0) {
    assert(ga->ps < maxprogramcap);
    String_view line = str_triml(str_split(&src, '\n'));
    if (line.size > 0 && *line.data != '#') {
      ga->p[ga->h.ps++] = parse_line(line);
    }
  }
  ga->h.main_f = 0;
}
