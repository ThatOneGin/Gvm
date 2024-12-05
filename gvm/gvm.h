#ifndef GVM_H
#define GVM_H
#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define maxstacksize 1024
#define maxprogramcap 1024

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
    dup,
    call
  }kind;
  int op;
  char *call_label;
}inst;

//typedef struct {
//  enum {
//    gen_char,
//    gen_int
//  }gen_type;
//  union {
//    char *str_value;
//    int int_value;
//  }b;
//}Psc_generic;

typedef struct {
  char *name;
  inst *code;
  int c_size;
}Label;

typedef struct {
  int stack[maxstacksize];
  int sp; // stack pointer
  int ip; // instruction pointer

  inst program[maxprogramcap];
  int programsize;

  bool running;
  Label *label_table;
  int lp; // label pointer
}VirtualMachine;

VirtualMachine alloc_vm(VirtualMachine *vm);
Trap run_inst(VirtualMachine *vm, inst i);
void err(char *msg, int level, bool _exit);
void run_program(VirtualMachine *vm, inst *program);
void dump_stack(VirtualMachine vm);
void write_file(inst *p, size_t size, char *filename);
inst *read_file(char *filename);
void push_label(VirtualMachine *v, char *name, inst *code, int size);
Label look_up_label(VirtualMachine v, char *name);
#endif

#ifdef GVM_IMPLEMENTATION

VirtualMachine vm = {0};

#define arr_size(arr) (sizeof((arr)) / sizeof((arr[0])))
#define instpush(value) (inst){.kind = push, .op = (value)}
#define instadd (inst){.kind = add}
#define instsub (inst){.kind = sub}
#define instdiv (inst){.kind = divi}
#define instmul (inst){.kind = mul}
#define insthalt (inst){.kind = halt}
#define instdup (inst){.kind = dup}
#define instjmp(addr) (inst){.kind = jmp, .op = (value)}
#define instprint (inst){.kind = print}
#define instcall(label) (inst){.kind = call, .call_label = label}

VirtualMachine alloc_vm(VirtualMachine *vm) {
  vm->ip = 0;
  vm->sp = 0;
  vm->running = false;
  vm->label_table = calloc(sizeof(Label), 16);
  return *vm;
}

Trap run_inst(VirtualMachine *vm, inst i) {
  Label lbl;
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
    printf("%d\n", vm->stack[vm->sp-1]);
    break;
  case jmp:
    vm->ip = i.op;
    break;
  case dup:
  vm->stack[vm->sp] = vm->stack[vm->sp-1];
  vm->sp += 1;
  break;
  case call:
    lbl = look_up_label(*vm, i.call_label);
    for (int j = 0; j <= lbl.c_size; j++) {
      run_inst(vm, lbl.code[j]);
    }
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
  int sec_c = 0;
  while (vm->running) {
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
  if (vm.sp > 0) {
    for (int i = 0; i < vm.sp; i++) {
      printf("%d: %d\n", i, vm.stack[i]);
    }
  } else {
    printf("{Empty}\n");
  }
}

void write_file(inst *p, size_t size, char *filename) {
  FILE *f;

  f = fopen(filename, "wb");

  if (!f) {
    perror("[gvm]: Cannot open output file");
    exit(1);
  }

  fwrite(p, sizeof(inst), size, f);
  fclose(f);
}

inst *read_file(char *filename) {
  FILE *f;

  f = fopen(filename, "rb");

  if (!f) {
    perror("[gvm]: Cannot open input file");
    exit(1);
  }

  fseek(f, 0L, SEEK_END);
  size_t c = ftell(f) / sizeof(inst);
  rewind(f);

  inst *program = malloc(sizeof(inst) * c);

  if (program == NULL) {
    perror("[gvm]: Cannot allocate memory for program. exiting");
    exit(1);
  }

  inst instruction;
  for (size_t i  = 0; i < c; i++) {
    fread(&instruction, sizeof(inst), 1, f);
    program[i] = instruction;
  }

  fclose(f);

  return program;
}

void push_label(VirtualMachine *v, char *name, inst *code, int size) {
  assert(v->lp <= 10);
  v->label_table[v->lp] = (Label) {
    .name = name,
    .code = code,
    .c_size = size
  };
  v->lp += 1;
}

Label look_up_label(VirtualMachine v, char *name) {
  for (int i = 0; i < v.lp; i++) {
    if (strcmp(v.label_table[i].name, name) == 0) {
      return v.label_table[i];
    }
  }

  printf("Couldn't find label named `%s`.\n", name);
  exit(1);
}
#endif
