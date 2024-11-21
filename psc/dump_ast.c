#include <stdio.h>
#include "parser.h"

static void dump_unit_ast(Ast_T *a, int indent) {
  for (int i = 0; i < indent; i++) {
    printf(" ");
  }

  switch (a->kind) {
  case AstIdent:
    printf("kind: %d value: %s\n", a->kind,((AstIdentifier *)a)->value);
    break;
  case AstInt:
    printf("kind: %d value: %d\n", a->kind, ((AstNumber *)a)->value);
    break;
  case Expr:
    printf("Kind: %d\n", a->kind);
    dump_unit_ast(((AstExpr *)a)->lhs, indent++);
    printf("Operator: %s\n", ((AstExpr *)a)->op);
    dump_unit_ast(((AstExpr *)a)->rhs, indent++);
    break;
  case Stmt:
    printf("Statement\n");
    break;
  default:
    printf("Cannot recognize node kind.\n");
    break;
  }
}
void dump_ast(AstList a) {
  for (int i = 0; i < a.size+1; i++) {
    dump_unit_ast(a.data[i], 0);
  }
}