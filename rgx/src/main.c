#include "../include/rgx.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int count;

int parse_opt(int argc, char **argv,String *regex,String *input,String *file){

  
  String *arg = malloc(argc * sizeof(String));
  for (int i = 0; i < argc; i++) {
      arg[i] = str_init(argv[i], 1064);
  }
    
  int counter = 0;
  
  for(int i = 0; i < argc; i++){
    if(str_cmpr(arg[i],"-f",5) == 0 && i + 1 < argc){
      i++;
      counter++;
      (*file) = arg[i];
    }else
    if(str_cmpr(arg[i],"-r",5) == 0 && i + 1 < argc){
      i++;
      counter++;
      (*regex) = arg[i];
    }else
    if(str_cmpr(arg[i],"-t",5) == 0 && i + 1 < argc){
      i++;
      counter++;
      (*input) = arg[i];
    }
  }
  if (argc == 2 && (*input).len == 0 && (*regex).len == 0 && (*file).len == 0){
    (*regex) = arg[0];
    (*input) = arg[1];
    return 0;
  }

  if (counter < 2){
    printf("Not enought parameters passed.\n");
    printf("Usage:\n\trgx -r [regex] -f [filename]\n");
    printf("Or:\n\trgx -r [regex] -t [text]\n");
    return -1;
  }
  if ((*regex).len == 0){
    printf("No regex given\n");
    printf("Usage:\n\trgx -r [regex] -f [filename]\n");
    printf("Or:\n\trgx -r [regex] -t [text]\n");
    return -1;
  }
  if((*file).len != 0 && (*input).len != 0){
    printf("A name file and a text where inserted to match\nChose one:\n1) File\n2) Text\n-> ");
    int n = 0;
    while(1){
      scanf("%d",&n);
      if(n == 1){
        str_reset(&(*input));
        break;
      }else
      if(n == 2){
        str_reset(&(*file));
        break;
      }else{
        printf("Not a valid choise\n-> ");
      }
    }
  }
  return 0;
}


int main(int argc, char **argv) {

  count = 0;
  String regex = str_init("", 0);
  String input = str_init("", 0);
  String file = str_init("", 0);
  if(parse_opt(argc - 1, &argv[1], &regex, &input, &file) == -1){
    return 1;
  }
  
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
        perror("Error opening file");
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
