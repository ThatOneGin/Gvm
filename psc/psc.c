#include "lex.c"
#include "parser.c"
#include "parser.h"
#include "gen.c"
#include "../gvm/gvm.h"

char *openfile(char *filename) {
  char * buffer = 0;
  long length;
  FILE * f = fopen (filename, "rb");

  if (f) {
    fseek (f, 0, SEEK_END);
    length = ftell (f);
    fseek (f, 0, SEEK_SET);
    buffer = malloc(length);
    if (buffer) {
      fread (buffer, 1, length, f);
    }
    fclose (f);
  }

  if (buffer) {
    return buffer;
  } else {
    printf("Couldn't open file due to memory allocation issue. Exiting.\n");
    exit(1);
  }
}

int main(int argc, char **argv) {
  if (argc < 1) {
    printf("Usage: ./psc <input file>\n");
    exit(1);
  }
  
  char *fl = openfile(argv[1]); // for now, only one file
  Lexer l = init_lexer(fl);
  tokenize(&l);
  Parser p = init_parser(l.tokens);
  AstList ast = make_ast(&p);

  evaluate_list(ast);
  append_to_program(insthalt);
  write_file(vm.program, psize);

  freelist(l);
  free_ast_list(ast);
  free(fl);
  return 0;
}
