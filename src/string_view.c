#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include "string_view.h"

String_view to_str(char *data) {
  return (String_view) {
    .data = data,
    .size = strlen(data)
  };
}

int str_cmp(String_view a, String_view b) {
  if (a.size != b.size) {
    return 0;
  } else {
    return memcmp(a.data, b.data, b.size) == 0;
  }
}

int str_atoi(String_view s) {
  int res = 0;
  for (size_t i = 0; i < s.size && isdigit(s.data[i]); i++) {
    res = res * 10 + s.data[i] - '0';
  }
  return res;
}

String_view str_triml(String_view str) {
  size_t i = 0;
  while (isspace(str.data[i]) && i < str.size) {
    i += 1;
  }

  return (String_view) {
    .size = str.size - i,
    .data = str.data + i
  };
}

String_view str_trimr(String_view s) {
  size_t i = 0;
  while (i < s.size && isspace(s.data[s.size - 1 - i])) {
    i += 1;
  }

  return (String_view) {
    .size = s.size - i,
    .data = s.data
  };
}

String_view str_cpy(String_view s) {
  return (String_view) {
    .size = strlen(s.data),
    .data = s.data
  };
}

String_view str_split(String_view *str, char delim) {
  size_t i = 0;
  while (i < str->size && str->data[i] != delim) {
    i += 1;
  }

  String_view res = {
    .size = i,
    .data = str->data
  };

  if (i < str->size) {
    str->size -= i + 1;
    str->data  += i + 1;
  } else {
    str->size -= i;
    str->data  += i;
  }

  return res;
}

String_view str_trimlr(String_view s) {
  return str_trimr(str_triml(s));
}

String_view str_trims(String_view str) {
  size_t i = 0;
  while (!isspace(str.data[i]) && i < str.size) {
    i += 1;
  }

  return (String_view) {
    .size = str.size - (i + 1),
    .data = str.data + i + 1
  };
}