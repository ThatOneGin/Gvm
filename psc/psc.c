#include "dump_ast.c"
#include "lex.c"
#include "parser.c"
#include "parser.h"
#include "gen.c"
#include "../gvm/gvm.h"
#include <stdio.h>

char *openfile(char *filename) {
  FILE *f = fopen(filename, "r");
  if (f == NULL) {
    perror("Error opening file");
    exit(1);
  }

  fseek(f, 0, SEEK_END);
  long f_size = ftell(f);
  fseek(f, 0, SEEK_SET);


  char *buffer = (char *)malloc(f_size + 1);
  if (buffer == NULL) {
    perror("Memory allocation failed");
    fclose(f);
    exit(1);
  }

  fread(buffer, 1, f_size, f);

  buffer[f_size] = '\0';

  fclose(f);
  return buffer;
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

  dump_ast(ast);

  freelist(l);
  free_ast_list(ast);
  free(fl);
  return 0;
}
