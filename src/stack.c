#include "stack.h"
#include <stdio.h>
#include <stdlib.h>

Stack stack_init(void) {
  Stack s = malloc(sizeof(Stack_value)*minstack);
  if (s == NULL) {
    printf("[Error]: Couldn't allocate stack.\n");
    exit(1);
  }
  return s;
}

void stack_realloc(Stack *s, size_t size) {
  Stack s2 = realloc(*s, size);
  if (s2 == NULL) {
    printf("[Error]: Couldn't reallocate stack.\n");
    free(s);
    exit(1);
  }
  *s = s2;
}

void stack_free(Stack *s) {
  free(s);
  s = NULL;
}
