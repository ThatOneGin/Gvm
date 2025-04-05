#ifndef STACK_H
#define STACK_H
#include <stdint.h>
#include <stdio.h>

#define minstack 5

typedef uintptr_t Stack_value;
typedef Stack_value *Stack;

Stack stack_init(void);
void stack_realloc(Stack *s, size_t size);
void stack_free(Stack *s);
#endif
