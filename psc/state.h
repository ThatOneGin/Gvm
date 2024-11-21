#ifndef Psc_state_header
#define Psc_state_header
#include "../gvm/gvm.h"

typedef struct {
  char *name;
  Psc_generic value;
  int type;
} psc_variable;

typedef struct {
  psc_variable *variables;
  size_t size;
  size_t capacity;
}Psc_State;

int set_variable(Psc_State *P, char *name, int type, Psc_generic value);
psc_variable get_variable(Psc_State *P, char *name);
int reset_variable(Psc_State *P, char *name, Psc_generic value);
#endif
