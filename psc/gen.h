#ifndef Psc_gen_
#define Psc_gen_

#include "parser.h"

typedef struct {
  enum {
    gen_char,
    gen_int
  }gen_type;
  union {
    char *str_value;
    int int_value;
  }b;
}Psc_generic;

Psc_generic ev_binop(AstExpr *ast);
Psc_generic evaluate(Ast_T *ast);
#endif
