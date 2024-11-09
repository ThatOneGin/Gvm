#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "gvm.h"

#define arr_size(arr) (sizeof((arr)) / sizeof((arr[0])))
#define maxstacksize 1 << 16

#define instpush(value) (inst){.kind = push, .op = (value)}
#define instadd (inst){.kind = add}
#define instsub (inst){.kind = sub}
#define instdiv (inst){.kind = divi}
#define instmul (inst){.kind = mul}
#define insthalt (inst){.kind = halt}
#define instprint (inst){.kind = print}

VirtualMachine alloc_vm() {
  VirtualMachine vm;
  vm.stack = calloc(maxstacksize, sizeof(vm.stack[0]));
  vm.ip = 0;
  vm.sp = 0;
  vm.running = false;
  return vm;
}

Trap run_inst(VirtualMachine *vm, inst i) {
  switch (i.kind) {
  case push: {
    vm->stack[vm->sp] = i.op;
    vm->sp += 1;
    break;
  }
  case add: {
    int a = vm->stack[vm->sp-1];
    int b = vm->stack[vm->sp-2];
    vm->stack[vm->sp] = (a + b);
    vm->sp += 1;
    break;
  }
  case halt:
    vm->running = false;
    return OK;
  case sub: {
    int a = vm->stack[vm->sp-1];
    int b = vm->stack[vm->sp-2];
    vm->stack[vm->sp] = (b - a);
    vm->sp += 1;
    break;
  }
  case mul: {
    int a = vm->stack[vm->sp-1];
    int b = vm->stack[vm->sp-2];
    vm->stack[vm->sp] = (a * b);
    vm->sp += 1;
    break;
  }
  case divi: {
    int a = vm->stack[vm->sp-1];
    int b = vm->stack[vm->sp-2];
    vm->stack[vm->sp] = (b / a);
    vm->sp += 1;
    break;
  }
  case print:
    printf("%d\n", vm->stack[vm->sp-1]); // don't pop it to use later
    break;
  default:
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
  while (vm->running) {
    Trap out = run_inst(vm, program[vm->ip]);
    if (out == OpNotRec) {
      err("Operand not recognized", 2, true);
    }
    vm->ip += 1;
  }
}

void dump_stack(VirtualMachine vm) {
  printf("VM stack:\n");
  if (vm.sp > 0) {
    for (int i = 0; i < vm.sp; i++) {
      printf("%d: %d\n", i, vm.stack[i]);
    }
  } else {
    printf("{Empty}\n");
  }
}

void write_file(inst *p, size_t size) {
  FILE *f;

  f = fopen("a.gvm", "wb");

  if (!f) {
    perror("Cannot open output file");
    exit(1);
  }

  fwrite(p, sizeof(inst), size, f);
  fclose(f);
}

inst *read_file(char *filename) {
  FILE *f;

  f = fopen(filename, "rb");

  if (!f) {
    perror("[gvm] Cannot open input file");
    exit(1);
  }

  fseek(f, 0L, SEEK_END);
  size_t c = ftell(f) / sizeof(inst);
  rewind(f);

  inst *program = malloc(sizeof(inst) * c);

  if (program == NULL) {
    perror("Cannot allocate memory for program. exiting");
    exit(1);
  }

  inst instruction;
  for (size_t i  = 0; i < c; i++) {
    fread(&instruction, sizeof(inst), 1, f);
    printf("%d %d\n", instruction.kind, instruction.op);
    program[i] = instruction;
  }

  fclose(f);

  return program;
}

int main(int argc, char **argv) {
  VirtualMachine vm = alloc_vm();
  vm.running = true;
  
  inst program[] = {
    instpush(54),
    instpush(2),
    instdiv,
    instprint,
    insthalt
  };

  //inst *p = read_file(argv[1]);

  run_program(&vm, program);
  dump_stack(vm);

  //free(p);
  free(vm.stack);
  return 0;
}