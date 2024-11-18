#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lex.h"

Lexer init_lexer(char *src) {
  Lexer l;
  l.src = src;
  l.srclen = strlen(l.src);
  l.index = 0;
  l.current = l.src[l.index];
  l.tokens = (TokenList) {0};
  l.tokens.data = malloc(sizeof(Token) * 15);
  l.tokens.size = 0;
  l.tokens.capacity = 15;

  if (l.tokens.data == NULL) {
    printf("ERROR: Cannot allocate memory for tokens array");
    exit(1);
  }

  return l;
}

void advance(Lexer *l) {
  l->index += 1;
  l->current = l->src[l->index];
}

void push_token(Lexer *l, Token t) {
  assert(l->tokens.size < l->tokens.capacity + 1); // fixed size for now
  l->tokens.data[l->tokens.size] = t;
  l->tokens.size += 1;
}

void tokenize(Lexer *l) {
  while (l->current != '\0' || l->index < l->srclen) {
    if (isspace(l->current)) {
      while (isspace(l->current)) {
        advance(l);
      }
    } else if (isalpha(l->current)) {
      char identifier[256] = "";
      int i = 0;
      
      while (isalpha(l->current) && i < 256) {
        identifier[i] = l->current;
        advance(l);
        i += 1;
      }
      
      identifier[i] = '\0';

      push_token(l, (Token) {.type = Identifier, .value = strdup(identifier)});
    } else if (isdigit(l->current)) {
      char digit[256] = "";
      int i = 0;
      
      while (isdigit(l->current) && i < 256) {
        digit[i] = l->current;
        advance(l);
        i += 1;
      }
      
      digit[i] = '\0';

      push_token(l, (Token) {.type = Number, .value = strdup(digit)});
    }

    switch (l->current) {
      case '+':
        advance(l);
        push_token(l, (Token) {.type = Operator, .value = strdup("+")});
        break;
      case '-':
        advance(l);
        push_token(l, (Token) {.type = Operator, .value = strdup("-")});
        break;
      case '*':
        advance(l);
        push_token(l, (Token) {.type = Operator, .value = strdup("*")});
        break;
      case '/':
        advance(l);
        push_token(l, (Token) {.type = Operator, .value = strdup("/")});
        break;
    }
  }

  push_token(l, (Token) { .type = PSC_EOF, .value = "EOF"});
}

void freelist(Lexer l) {
  for (size_t i = 0; i < l.tokens.size-1; i++) {
    free(l.tokens.data[i].value);
  }
  free(l.tokens.data);
}