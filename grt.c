#include "gvm.c"

int main(int argc, char **argv) {
  if (argc < 2) {
    printf("Expected input file.\n");
    exit(1);
  }
  vm.running = true;

  inst *p = read_file(argv[1]);

  run_program(&vm, p);
  dump_stack(vm);

  free(p);
  return 0;
}