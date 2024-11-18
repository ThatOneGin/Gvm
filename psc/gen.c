#include "../gvm/gvm.c"
#include "gen.h"
#include "parser.h"
#include <stdlib.h>
#include <stdio.h>

static size_t psize = 0;

static void append_to_program(inst i) {
  assert(psize < maxprogramcap);
  vm.program[psize] = i;
  psize += 1;
}

Psc_generic evaluate(Ast_T *ast) {
  switch (ast->kind) {
  case AstInt:
    AstNumber *a = (AstNumber *) ast;
    return (Psc_generic) {
      .gen_type = gen_int,
      .b.int_value = a->value
    };
  case AstIdent:
    printf("TODO: not implemented\n");
    exit(1);
  case Expr:
    return ev_binop((AstExpr *) ast);
  default:
    printf("Type not recognized: %d", ast->kind);
    exit(1);
  }
}

Psc_generic ev_binop(AstExpr *ast) {
  Psc_generic lhs = evaluate(ast->lhs);
  Psc_generic rhs = evaluate(ast->rhs);

  append_to_program(instpush(lhs.b.int_value));
  append_to_program(instpush(rhs.b.int_value));

  if (strcmp(ast->op, "+") == 0) {
    append_to_program(instadd);
    return (Psc_generic) {
      .gen_type = gen_int,
      .b.int_value = (lhs.b.int_value + rhs.b.int_value)
    };
  } else if (strcmp(ast->op, "-") == 0) {
    append_to_program(instsub);
    return (Psc_generic) {
      .gen_type = gen_int,
      .b.int_value = (lhs.b.int_value - rhs.b.int_value)
    };
  } else if (strcmp(ast->op, "/") == 0) {
    append_to_program(instdiv);
    return (Psc_generic) {
      .gen_type = gen_int,
      .b.int_value = (lhs.b.int_value / rhs.b.int_value)
    };
  } else {
    append_to_program(instmul);
    return (Psc_generic) {
      .gen_type = gen_int,
      .b.int_value = (lhs.b.int_value * rhs.b.int_value)
    };
  }
}

void evaluate_list(AstList a) {
  for (int i = 0; i < a.size + 1; i++) {
    evaluate(a.data[i]);
  }
}
