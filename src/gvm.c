#include "gvm.h"
#include "instruction.h"
#include "stack.h"
#include <assert.h>
#include <stdio.h>

#define arith(op)   \
  decptr(vm->last); \
  uintptr_t lhs = vm->stack[abs((int)(vm->last-vm->stack))]; \
  decptr(vm->last); \
  uintptr_t rhs = vm->stack[abs((int)(vm->last-vm->stack))]; \
  adjust_stack(vm); \
  int entry = abs((int)(vm->last - vm->stack));\
  vm->stack[entry] = lhs op rhs;\
  incptr(vm->last);


VirtualMachine alloc_vm(VirtualMachine *vm) {
  vm->halt = false;
  vm->stack = stack_init();
  vm->top = vm->stack+minstack;
  vm->last = vm->stack;
  return *vm;
}

void vm_stack_realloc(VirtualMachine *vm) {
  int new_stack_size = abs(stacksize(vm)*2);
  stack_realloc(&vm->stack, sizeof(Stack_value)*(new_stack_size));
  vm->top = vm->stack + new_stack_size;
}

void adjust_stack(VirtualMachine *vm) {
  int offset = abs((int)(vm->last-vm->top));
  if (offset <= 1) {
    vm_stack_realloc(vm);
  }
}

Trap run_inst(VirtualMachine *vm, inst i) {
  switch (i.kind) {
  case push: {
    adjust_stack(vm);
    int entry = diff(vm->last, vm->stack);
    vm->stack[entry] = i.op;
    incptr(vm->last);
    break;
  } case jmp:
    vm->ip = i.op;
    break;
  case print: {
    decptr(vm->last);
    int entry = diff(vm->last, vm->stack);
    printf("%ld", vm->stack[entry]);
    break;
  } case add: {
    assert(diff(vm->last-1, vm->stack) > 0);
    arith(+);
  } case sub: {
    assert(diff(vm->last-1, vm->stack) > 0);
    arith(-);
  } case divi: {
    assert(diff(vm->last-1, vm->stack) > 0);
    arith(/);
  } case mul: {
    assert(diff(vm->last-1, vm->stack) > 0);
    arith(*);
  } default:
    return OpNotRec;
  }

  return OK;
}

void err(char *msg, int level, bool _exit) {
  printf("[gvm]: %s level: %d\n", msg, level);
  if (_exit) {
    exit(1);
  }
}

void run_program(VirtualMachine *vm, inst *program) {
  int sec_c = 0;
  while (!vm->halt) {
    if (sec_c > 2000) {
      err("Maximum iterations reached.", 3, true);
      break;
    }
    Trap out = run_inst(vm, program[vm->ip]);
    if (out == OpNotRec) {
      err("Operand not recognized", 2, true);
    }
    vm->ip += 1;
    sec_c += 1;
  }
}

void dump_stack(VirtualMachine vm) {
  printf("GVM stack:\n");
  int len = abs(stacksize(&vm));

  if (len > 0) {
    for (int i = 0; i < len; i++) {
      printf("  %d: %ld\n", i, vm.stack[i]);
    }
  } else {
    printf("{Empty}\n");
  }
}

void close_vm(VirtualMachine *vm) {
  free(vm->stack);
  vm->top = NULL;
  vm->last = NULL;
  vm->halt = true;
}

int main(int argc, char **argv) {
  assert(argc >= 2);
  VirtualMachine vm = {0};
  alloc_vm(&vm);
  /* TODO: make assembler */
  close_vm(&vm);
  return 0;
}
