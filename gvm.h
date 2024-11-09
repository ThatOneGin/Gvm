#ifndef GVM_H
#define GVM_H
#include <stdbool.h>
#include <stdlib.h>

typedef struct {
  char *data;
  size_t i;
}String;

typedef struct {
  String *str;
  size_t p;
  size_t item_count;
}StrP;

typedef enum {
  OK,
  StackOverflow,
  StackUnderflow,
  OpNotRec
}Trap;

typedef struct {
  enum {
    push,
    add,
    halt,
    mul,
    sub,
    divi,
    print
  }kind;
  int op;
}inst;

typedef struct {
  unsigned char *memory;
  int *stack;
  int sp;
  int ip;

  bool running;
}VirtualMachine;
#endif