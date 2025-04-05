#ifndef INSTRUCTION_H
#define INSTRUCTION_H
#include <stdint.h>
typedef enum {
  push,
  add,
  halt,
  mul,
  sub,
  divi,
  shout,
  jmp,
} insttype;

typedef struct {
  insttype kind;
  uintptr_t op;
}inst;
#endif
