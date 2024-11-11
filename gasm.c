#include "gvm.c"
#include "gvm.h"

int main(int argc, char **argv) {
  if (argc < 2) {
    printf("Expected input file.\n");
    exit(1);
  }

  String code = file_to_str(argv[1]);

  vm.programsize = parse_src(code, vm.program, maxprogramcap);
  write_file(vm.program, vm.programsize);
  return 0;
}