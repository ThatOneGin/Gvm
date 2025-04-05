#ifndef IO_H
#define IO_H
#include "instruction.h"
#include "string_view.h"
#include "asm.h"
#include <stdio.h>

String_view openfile(char *filepath);
void write_file(Gassembly ga, char *filename);
Gassembly read_file(char *filename);
#endif