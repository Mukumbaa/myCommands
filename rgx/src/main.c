#include "../include/rgx.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int count;

void parse_opt(int argc, char **argv){
  printf("%d\n",argc);
  for(int i = 0; i < argc; i++){
    printf("%s\n",argv[i]);
  }

  

  
}


int main(int argc, char **argv) {

  count = 0;

  

  parse_opt(argc, &argv[1]);
  return 1;
  
  String *arg = malloc(argc * sizeof(String));
  for(int i = 1; i < argc; i++){
    arg[i-1] = str_init(argv[i], 1064);
  }
  String regex = str_init("", 0);
  String input = str_init("", 0);
  String file = str_init("", 0);
  for(int i = 0; i < argc - 1; i++){
    if(str_cmpr(arg[i], "-f", 3) == 0 && i + 1 < argc - 1){
      // printf("qui\n");
      i++;
      file = arg[i];
    }else if (str_cmpr(arg[i], "-r", 3) == 0 && i + 1 < argc - 1) {
      i++;
      regex = arg[i];
    }else{
      if (regex.len == 0){
        regex = arg[i];
      }else if (input.len == 0){
        input = arg[i];
      }
    }
  }

  // return -1;


  // printf("%s\n",regex.str);
  // printf("%s\n",input.str);
  // printf("%s\n",file.str);

  // regex = str_init(argv[argc - 2], 100);
  // input = str_init(argv[argc - 1], 100);
  int size = 0;
  regex_e *list = NULL;
  list = malloc(size * sizeof(regex_e));
  clock_t start,end;
  double time;
  start = clock();
  if (parse_regex(regex, &list, &size) == Err) {
    return -1;
  }
  if(sanitize_regex(list, size) == false){
    printf("Bad formulated regex\n");
    return -1;
  }
  end = clock();
  time = ((double) end - start) /CLOCKS_PER_SEC;
  printf("parse time: %f\n",time);
  // for (int i = 0; i < size; i++) {
  //   printf("%s-%s\n", list[i].str.str, string_regex_op(list[i].r_op).str);
  // }

  start = clock();

  if(file.len != 0){

    FILE *fp = fopen(file.str, "r");  // Apri il file in modalità lettura
    if (fp == NULL) {
        perror("Errore nell'apertura del file");
        return 1;
    }

    char buffer[1064];

    count = 0;
    while(fgets(buffer, 1064, fp) != NULL){
      input = str_init(buffer, 1064);
      input.str[input.len - 1] = '\0';
      match_regex_anywhere(list, size, input);
    }

  }else if (input.len != 0){
    count = 0;
    match_regex_anywhere(list, size, input);
  }
  if (count == 0){
    printf("No match\n");
  }







  
  end = clock();
  time = ((double) end - start) /CLOCKS_PER_SEC;
  // printf("%d\n", r);
  printf("match time: %f\n",time);
  return 0;
}
