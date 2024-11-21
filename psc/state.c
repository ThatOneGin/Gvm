#include "state.h"
#include <stdio.h>

int set_variable(Psc_State *P, char *name, int type, Psc_generic value) {
  if (P->size > P->capacity) {
    return -1;
  }

  P->variables[P->size] = (psc_variable) {
    .name = name,
    .type = type,
    .value = value
  };
  P->size += 1;

  return 0;
}
psc_variable get_variable(Psc_State *P, char *name) {
  for (size_t i = 0; i < P->size; i++) {
    if (P->variables[i].name == name) {
      return P->variables[i];
    }
  }

  printf("Couldn't find variable named %s", name);
  exit(1);
}

int reset_variable(Psc_State *P, char *name, Psc_generic value) {
  for (size_t i = 0; i < P->size; i++) {
    if (P->variables[i].name == name) {
      P->variables[i].value = value;
      return 0;
    }
  }

  return -1;
}
