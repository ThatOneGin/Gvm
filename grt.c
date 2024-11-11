#include "gvm.c"

int main(int argc, char **argv) {
  vm.running = true;

  inst *p = read_file(argv[1]);

  run_program(&vm, p);
  dump_stack(vm);

  free(p);
  return 0;
}