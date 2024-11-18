#ifndef Psc_parser_header
#define Psc_parser_header
#include "lex.h"
typedef struct {
  enum {
    Expr, // 0
    Stmt, // 1
    AstIdent, // 2
    AstInt // 3
  }kind;
}Ast_T;

typedef struct {
  Ast_T base;
  int value;
}AstNumber;

typedef struct {
  Ast_T base;
  char *value;
}AstIdentifier;

typedef struct {
  Ast_T base;
  Ast_T *lhs;
  Ast_T *rhs;
  char *op;
}AstExpr;

typedef struct {
  int size;
  Ast_T **data;
  int capacity;
}AstList;

typedef struct {
  TokenList tokens;
  int index;
}Parser;

Token parser_eat(Parser *p);
Token parser_at(Parser *p);
Ast_T *parse_primary_expr(Parser *p);
Ast_T *parse_multiplicitave_expr(Parser *p);
Ast_T *parse_additive_expr(Parser *p);
Ast_T *parse_stmt(Parser *p); // todo
#endif
