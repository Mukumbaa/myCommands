#include "../include/rgx.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char **argv) {

  String regex = str_init(argv[argc - 2], 100);
  String input = str_init(argv[argc - 1], 100);
  int ret;
  int size = 0;
  regex_e *list = NULL;
  list = malloc(size * sizeof(regex_e));
  clock_t start,end;
  double time;
  start = clock();
  ret = parse_regex(regex, &list, &size);
  if (ret == Err) {
    return -1;
  }
  end = clock();
  time = ((double) end - start) /CLOCKS_PER_SEC;
  printf("parse time: %f\n",time);
  for (int i = 0; i < size; i++) {
    printf("%s-%s\n", list[i].str.str, string_regex_op(list[i].r_op).str);
}

  start = clock();
  /*bool r = */match_regex_anywhere(list, size, input);
  end = clock();
  time = ((double) end - start) /CLOCKS_PER_SEC;
  // printf("%d\n", r);
  printf("match time: %f\n",time);
  return 0;
}
