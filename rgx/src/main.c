#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../include/libstr.h"

typedef enum ret{
  Ok = 0,
  Err = 1,
}ret;

typedef enum regex_op{
  star = 1,
  question = 2,
  plus = 3,
  dot = 4,
  start_str = 5,
  end_str = 6,
  set = 7,//
  group = 8,
  alter = 9,
  range = 10,
  charr = 11,
  escape = 12,
}regex_op;

typedef struct regex_e{
  regex_op r_op;
  String str;
}regex_e;

int parse_regex(String regex, regex_e **list, int *size){
  String buffer;
  int len = 0;
  regex_op r_op;
  bool flag = false;
  int old_i = 0;
  
  

  for(int i = 0; i < regex.len; i++){
    str_reset(&buffer);
    len = 0;
    switch (regex.str[i]){
      case '^':
        len ++;
        str_char(&buffer, '^');
        r_op = start_str;
        break;
      case '$':
        len ++;
        str_char(&buffer, '$');
        r_op = end_str;
        break;
      case '.':
        len ++;
        str_char(&buffer, '.');
        r_op = dot;
        break;
      case '*':
        len ++;
        str_char(&buffer, '*');
        r_op = star;
        break;
      case '|':
        len ++;
        str_char(&buffer, '|');
        r_op = alter;
      case '\\':
        len ++;
        str_char(&buffer, '\\');
        r_op = escape;
        break;
      case '[':
        len ++;
        flag = false;
        old_i = i;
        str_char(&buffer, regex.str[i]);          
        for(i = i+1; i < regex.len && flag == false; i++){
          if (regex.str[i] == ']') flag = true;
          len ++;
          str_char(&buffer, regex.str[i]);          
        }
        if (flag == false){
          printf("Not closing set at index: %d\n%s\n", old_i,regex.str);
          for(i = 0; i < old_i; i++){
            printf(" ");
          }
          printf("^\n");
          return -1;
        }
        i--;
        r_op = set;
        break;
      case '(':
        len ++;
        flag = false;
        old_i = i;
        str_char(&buffer, regex.str[i]);          
        for(i = i+1; i < regex.len && flag == false; i++){
          if (regex.str[i] == ')') flag = true;
          len ++;
          str_char(&buffer, regex.str[i]);          
        }
        if (flag == false){
          printf("Not closing group at index: %d\n%s\n", old_i,regex.str);
          for(i = 0; i < old_i; i++){
            printf(" ");
          }
          printf("^\n");
          return -1;
        }
        i--;
        r_op = group;
        break;
      case '{':
        len ++;
        flag = false;
        old_i = i;
        str_char(&buffer, regex.str[i]);          
        for(i = i+1; i < regex.len && flag == false; i++){
          if (regex.str[i] == '}') flag = true;
          len ++;
          str_char(&buffer, regex.str[i]);          
        }
        if (flag == false){
          printf("Not closing range at index: %d\n%s\n", old_i,regex.str);
          for(i = 0; i < old_i; i++){
            printf(" ");
          }
          printf("^\n");
          return -1;
        }
        i--;
        r_op = range;
        break;
      default:
        len ++;
        str_char(&buffer, regex.str[i]);
        r_op = charr;
        // printf("default\n");
        // return Err;
        break;
    }
    (*size)++;
    (*list) = realloc((*list), (*size) * sizeof(regex_e));
    str_cpy(&(*list)[(*size) - 1].str, buffer);
    (*list)[(*size) - 1].r_op = r_op;
  }
  return Ok;
}
String string_regex_op(regex_op r_op){
  String str_r_op;
  switch (r_op) {
    case 1:
      str_r_op = str_init("star",5);
      break;
    case 2:
      str_r_op = str_init("question",10);
      break;
    case 3:
      str_r_op = str_init("plus",5);
      break;
    case 4:
      str_r_op = str_init("dot",5);
      break;
    case 5:
      str_r_op = str_init("start_str",15);
      break;
    case 6:
      str_r_op = str_init("end_str",15);
    case 7:
      str_r_op = str_init("set",15);
      break;
    case 8:
      str_r_op = str_init("group",15);
      break;
    case 9:
      str_r_op = str_init("alter",15);
      break;
    case 10:
      str_r_op = str_init("range",15);
      break;
    case 11:
      str_r_op = str_init("charr",15);
      break;
    case 12:
      str_r_op = str_init("escape",15);
      break;
  }
  return str_r_op;
}

int check_regex(regex_e *list, int size,String input){

  for (int i = 0; i < size; i++){
    
  }
    





  return Ok;
}

int main(int argc, char **argv){

  String regex = str_init(argv[1], 100);
  // String input = str_init(argv[2], 100);
  String input = str_init("ciao", 100);
  int size = 0;
  regex_e *list = NULL;
  list = malloc(size * sizeof(regex_e));
  int ret;
  ret = parse_regex(regex, &list, &size);
  if (ret == Err){
    return -1;
  }
  
  for(int i = 0; i < size; i++){
    printf("%s-%s\n",list[i].str.str, string_regex_op(list[i].r_op).str);
  }
  
  ret = check_regex(list, size, input);

  if (ret == Err){
    return -1;
  }
  
  return 0;
}
