#ifndef RGX
#define RGX

#include "../include/libstr.h"
#include <stdbool.h>
#include <stdio.h>

typedef enum ret {
  Ok = 0,
  Err = 1,
} ret;

typedef enum regex_op {
  star = 1,      //*
  question = 2,  //?
  plus = 3,      //+
  dot = 4,       //.
  start_str = 5, //^
  end_str = 6,   //$
  set = 7,       //[...]
  group = 8,     //()
  alter = 9,     //|
  range = 10,    //{...}
  charr = 11,    // any char
  escape = 12,   //'\'
} regex_op;
typedef enum action {
  zero = 0,
  one = 1,
  zeroOrOne = 2,
  zeroOrMore = 3,
  oneOrMore = 4,
  inRange = 5,
} action;

typedef struct regex_e {
  regex_op r_op;
  String str;
} regex_e;

int parse_regex(String regex, regex_e **list, int *size);
bool match_regex_anywhere(regex_e *regex, int size, String input);
String string_regex_op(regex_op r_op);

#endif
