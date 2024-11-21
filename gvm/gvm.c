#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "gvm.h"

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

VirtualMachine alloc_vm() {
  VirtualMachine vm;
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
    printf("%d\n", vm->stack[vm->sp-1]);
    break;
  case jmp:
    vm->ip = i.op;
    break;
  case dup:
  vm->stack[vm->sp] = vm->stack[vm->sp-1];
  vm->sp += 1;
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
    if (sec_c > 30) {
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
    program[i] = instruction;
  }

  fclose(f);

  return program;
}

String c_str(char *str) {
  return (String){
    .count = strlen(str),
    .data = str
  };
}

String trim_l(String str) {
  size_t i = 0;
  while (isspace(str.data[i]) && i < str.count) {
    i += 1;
  }

  return (String) {
    .count = str.count - i,
    .data = str.data + i
  };
}

String trim_r(String str) {
  size_t i = 0;
  while (i < str.count && isspace(str.data[str.count - 1 - i])) {
    i += 1;
  }

  return (String) {
    .count = str.count - i,
    .data = str.data
  };
}

String s_chop(String *str, char delim) {
  size_t i = 0;
  while (i < str->count && str->data[i] != delim) {
    i += 1;
  }

  String dest = {
    .count = i,
    .data = str->data
  };

  if (i < str->count) {
    str->count -= i + 1;
    str->data  += i + 1;
  } else {
    str->count -= i;
    str->data  += i;
  }

  return dest;
}

int s_cmp(String a, String b) {
  if (a.count != b.count) {
    return 0;
  } else {
    return memcmp(a.data, b.data, a.count) == 0;
  }
}

int s_atoi(String str) {
  int res = 0;
  for (size_t i = 0; i < str.count && isdigit(str.data[i]); i++) {
    res = res * 10 + str.data[i] - '0';
  }

  return res;
}

inst parse_line(String ln) {
  ln = trim_l(ln);
  String i_n = s_chop(&ln, ' ');

  if (s_cmp(i_n, c_str("push"))) {
    ln = trim_l(ln);
    int op = s_atoi(trim_l(ln));

    return (inst){.kind = push, .op = op};
  } else if (s_cmp(i_n, c_str("add"))) {
    ln = trim_l(ln);
    return (inst){.kind = add};
  } else if (s_cmp(i_n, c_str("sub"))) {
    ln = trim_l(ln);
    return (inst){.kind = sub};
  } else if (s_cmp(i_n, c_str("div"))) {
    ln = trim_l(ln);
    return (inst){.kind = divi};
  } else if (s_cmp(i_n, c_str("mul"))) {
    ln = trim_l(ln);
    return (inst){.kind = mul};
  } else if (s_cmp(i_n, c_str("print"))) {
    ln = trim_l(ln);
    return (inst){.kind = print};
  } else if (s_cmp(i_n, c_str("halt"))) {
    ln = trim_l(ln);
    return (inst) {.kind = halt};
  } else if (s_cmp(i_n, c_str("jmp"))) {
    ln = trim_l(ln);
    int op = s_atoi(trim_l(ln));

    return (inst){.kind = jmp, .op = op};
  } else if (s_cmp(i_n, c_str("dup"))) {
    ln = trim_l(ln);
    return (inst){.kind = dup};
  } else {
    printf("unknown instruction %s", i_n.data);
    exit(1);
  }
}

String file_to_str(char *filepath) {
  FILE *f = fopen(filepath, "r");
  if (f == NULL) {
    printf("Could not read input file.\n");
    exit(1);
  }

  if (fseek(f, 0, SEEK_END) < 0) {
    printf("Could not read input file.\n");
    exit(1);
  }

  size_t sz = ftell(f);
  if (sz <= 0) {
    printf("Could not read input file.\n");
    exit(1);
    exit(1);
  }

  char *buf = malloc(sz);
  if (buf == NULL) {
    printf("Could not alloc memory for file.\n");
    exit(1);
  }

  if (fseek(f, 0, SEEK_SET) < 0) {
    printf("Could not read input file.\n");
    exit(1);
  }

  size_t flsz = fread(buf, 1, sz, f);
  if (ferror(f)) {
    printf("Could not read input file.\n");
    exit(1);
  }

  fclose(f);

  return (String) {
    .count = flsz,
    .data = buf,
  };
}

size_t parse_src(String source, inst *program, size_t program_capacity) {
  size_t program_size = 0;
  
  while (source.count > 0) {
    assert(program_size < program_capacity);
    
    String line = trim_l(s_chop(&source, '\n'));
    
    if (line.count > 0) {
      program[program_size++] = parse_line(line);
    }
  }

  return program_size;
}

static void gvm_push_int(VirtualMachine *vm, int value) {
  vm->stack[vm->sp] = value;
  vm->sp += 1;
}
static int gvm_pop(VirtualMachine *vm) {
  assert(vm->sp > 0);
  return vm->stack[vm->sp-1];
}


//macros to manipulate the stack without having a program

#define psc_gvm_push_int(value) gvm_push_int(&vm, value);
#define psc_gvm_init()          vm.running = true;
#define psc_gvm_terminate()     vm.runnint = false;
#define psc_gvm_gettop()        vm.stack[vm.sp-1];
#define psc_gvm_pop()           gvm_pop(&vm);