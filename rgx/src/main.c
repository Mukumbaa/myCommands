#include "../include/rgx.h" //libstr.h stdbool.h
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {

  String regex = str_init(argv[1], 100);
  String input = str_init(argv[2], 100);
  int ret;
  int size = 0;
  regex_e *list = NULL;
  list = malloc(size * sizeof(regex_e));

  ret = parse_regex(regex, &list, &size);
  if (ret == Err) {
    return -1;
  }
  
  for (int i = 0; i < size; i++) {
    printf("%s-%s\n", list[i].str.str, string_regex_op(list[i].r_op).str);
  }

  bool r = match_regex_anywhere(list, size, input);
  printf("%d\n", r);

  return 0;
}
