#ifndef IO_H
#define IO_H
#include "instruction.h"
#include <stdio.h>

void write_file(inst *p, size_t size, char *filename);
inst *read_file(char *filename);
#endif