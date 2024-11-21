#ifndef GVM_H
#define GVM_H
#include <stdbool.h>
#include <stdlib.h>

#define maxstacksize 1024
#define maxprogramcap 1024

typedef struct {
  char *data;
  size_t count;
}String;

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
    print,
    jmp,
    dup
  }kind;
  int op;
}inst;

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

typedef struct {
  unsigned char *memory;
  int stack[maxstacksize];
  int sp; // stack pointer
  int ip; // instruction pointer

  inst program[maxprogramcap];
  int programsize;

  bool running;
}VirtualMachine;
#endif
