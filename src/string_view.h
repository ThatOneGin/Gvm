#ifndef STRING_VIEW_H
#define STRING_VIEW_H
#include <stdio.h>

typedef struct {
  char *data;
  size_t size;
} String_view;


typedef struct {
  char *data;
  size_t size;
} Str;

String_view str_capture(String_view s, char delim);
String_view to_str(char *data) ;
int str_cmp(String_view a, String_view b);
String_view str_split(String_view *s, char delim);
int str_atoi(String_view s);
String_view str_triml(String_view s);
String_view str_trimr(String_view s);
String_view str_trimlr(String_view s);
String_view str_trims(String_view str);
String_view str_cpy(String_view s);
#endif