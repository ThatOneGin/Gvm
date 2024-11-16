#include "gvm.c"

void usage() {
  printf("Grt, a runtime for the pytharscript language.\n");
  printf("Usage: ./grt <input.gvm>\n\n");
  printf("Error: input file not specified.\n");
}

int main(int argc, char **argv) {
  if (argc < 2) {
    usage();
    exit(1);
  }
  vm.running = true;

  inst *p = read_file(argv[1]);

  run_program(&vm, p);
  dump_stack(vm);

  free(p);
  return 0;
}