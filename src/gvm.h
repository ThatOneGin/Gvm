#ifndef GVM_H
#define GVM_H
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "instruction.h"
#include "stack.h"

#define maxstacksize 1024
#define maxprogramcap 1024

typedef enum {
  OK,
  StackOverflow,
  StackUnderflow,
  OpNotRec
}Trap;

typedef struct {
  Stack stack; /* stack base */
  Stack last; /* first free element */
  Stack top; /* last element */

  inst program[maxprogramcap];
  int programsize;
  int ip;

  bool halt;
}VirtualMachine;

VirtualMachine alloc_vm(VirtualMachine *vm);
void vm_stack_realloc(VirtualMachine *vm);
void adjust_stack(VirtualMachine *vm);
Trap run_inst(VirtualMachine *vm, inst i);
void err(char *msg, int level, bool _exit);
void run_program(VirtualMachine *vm, inst *program);
void dump_stack(VirtualMachine vm);
void close_vm(VirtualMachine *vm);

#define incptr(p) ((p)++)
#define decptr(p) ((p)--)
#define diff(p1, p2) (abs((int)(p1-p2)))
#define stacksize(vm) ((int)((vm)->stack-(vm)->top))
#endif
