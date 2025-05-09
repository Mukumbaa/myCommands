#include "../include/strlib.h"
// #include <stdexcept>


String str_init(const char *s, int max_len){

  String str;
  str.len = 0;
  int i = 0;
  for(i = 0; i < max_len && s[i] != '\0'; i++){
    str.len++;
  }

  i = 0;
  str.str = malloc(( str.len + 1 ) * sizeof(char));
  for(i = 0; i < str.len; i++){
    str.str[i] = s[i];
  }
  str.str[i] = '\0';

  return str;  
}
void str_reset(String *str){
  *str = str_init("",5);
}
void str_resetr(char **str, int len){
  for(int i = 0; i < len; i++){
    (*str)[i] = '\0';
  }
}

void str_cat(String *dest, const String src, int index){

  int new_len = dest->len + src.len;
  dest->str = realloc(dest->str, (new_len + 1) * sizeof(char));

  int i = 0;
  int j = 0;



  if(index >= 0 && index <= dest->len){
    for(i = dest->len; i >= index && i < new_len; i--){
      dest->str[i + src.len] = dest->str[i];
    }
    for(i = 0; i < src.len && i < new_len; i++){
      dest->str[index + i] = src.str[i];
    }
    dest->str[new_len + 1] = '\0';
    dest->len = new_len;
  }else{
    for (i = dest->len, j = 0; i <= new_len && j <= src.len; i++, j++) {
      dest->str[i] = src.str[j];
    }
    dest->str[i] = '\0';
    dest->len = new_len;
  }




  
  return;
}

void str_catr(String *dest, const char *src,  int max_len){

  int len = 0;
  for(int i = 0; i < max_len && src[i] != '\0'; i++){
    len++;
  }
  
  int new_len = dest->len + len ;
  dest->str = realloc(dest->str, (new_len) * sizeof(char));

  int i = 0;
  int j = 0;
  
  for (i = dest->len, j = 0; i <= new_len && j <= len; i++, j++) {
    dest->str[i] = src[j];
  }
  dest->str[i] = '\0';
  dest->len = new_len;
  return;
}

// Return 0 if equale, else 1
int str_cmp(String str1, String str2){

  if (str1.len != str2.len){
    return 1;
  }

  for(int i = 0; i < str1.len; i++){
    if(str1.str[i] != str2.str[i]){
      return 1;
    }
  }
  return 0;
}
int str_cmpr(String str1, char *str2, int max_len){
  int len = 0;
  for(int i = 0; i < max_len && str2[i] != '\0'; i++){
    len++;
  }

  if (str1.len != len){
    return 1;
  }

  for(int i = 0; i < str1.len; i++){
    if(str1.str[i] != str2[i]){
      return 1;
    }
  }
  return 0;
}

int str_contains(String str, String pattern, int is_case_sensitive){

  if(str.len < pattern.len){
    return -1;
  }
  int index = 0;
  int i;
  for (i = 0; i < str.len; i++){
    if(is_case_sensitive == NO_CASE_SENSITIVE ?
       tolower(str.str[i]) == tolower(pattern.str[index]) :
       str.str[i] == pattern.str[index]){

      if(index < pattern.len){
        index++;
      }
    }
    else if(index == pattern.len){
      return i - pattern.len;
    }
    else{
      index = 0;
    }
  }
  if(index == pattern.len){
    return i - pattern.len;
  }
  return -1;  
}
int str_containsr(String str, char *pattern, int max_len, int is_case_sensitive){

  int len = 0;
  for(int i = 0; i < max_len && pattern[i] != '\0'; i++){
    len++;
  }

  if(str.len < len){
    return -1;
  }

  int index = 0;
  int i;
  for (i = 0; i < str.len; i++){
    if(is_case_sensitive == NO_CASE_SENSITIVE ? tolower(str.str[i]) == tolower(pattern[index]) : str.str[i] == pattern[index]){
      if(index < len){
        index++;
      }
    }
    else if(index == len){
      return i - len;
    }
    else{
      index = 0;
    }
  }
  if(index == len){
    return i - len;
  }
  return -1;  
}

int str_split(String **arr, String str, char delimiter, int token_max_len){

  *arr = NULL;
  int dim = 0;
  int index = 0;
  char *token = malloc(token_max_len * sizeof(char));
  str_resetr(&token, token_max_len);

  
  
  for(int i = 0; i < str.len; i++){
    if(index > token_max_len - 1){
      printf("Token in split too big (max len: %d)\n", token_max_len);
      return -1;
    }

    if(str.str[i] == delimiter){
      if(index > 0){
        dim++;
        *arr = realloc(*arr, dim * sizeof(String));
        (*arr)[dim - 1] = str_init(token, index + 1);
        str_resetr(&token, token_max_len);
        index = 0;
      }
    }else{
      token[index] = str.str[i];
      index++;
    }
  }
  if(index > 0){
    dim++;
    *arr = realloc(*arr, dim * sizeof(String));
    (*arr)[dim - 1] = str_init(token, index + 1);
    str_resetr(&token, token_max_len);
  }
  return dim;
}
int str_splitr(String **arr, char *str, char delimiter, int token_max_len, int max_len){
  
  *arr = NULL;
  int dim = 0;
  int index = 0;
  char *token = malloc(token_max_len * sizeof(char));
  str_resetr(&token, token_max_len);

  int len = 0;
  for(int i = 0; i < max_len && str[i] != '\0'; i++){
    len++;
  }
  
  
  for(int i = 0; i < len && str[i] != '\0'; i++){
    if(index > token_max_len - 1){
      printf("Token in split too big (max len: %d)\n", token_max_len);
      return -1;
    }

    if(str[i] == delimiter){
      if(index > 0){
        dim++;
        *arr = realloc(*arr, dim * sizeof(String));
        (*arr)[dim - 1] = str_init(token, index + 1);
        index = 0;
        str_resetr(&token, token_max_len);
      }
    }else{
      token[index] = str[i];
      index++;
    }
  }
  if(index > 0){
    dim++;
    *arr = realloc(*arr, dim * sizeof(String));
    (*arr)[dim - 1] = str_init(token, index + 1);
    str_resetr(&token, token_max_len);
  }
  return dim;

}
void str_cpy(String *dest, const String src){
  
  printf("strcpy\n");
  *dest = str_init(src.str, src.len + 1);
  return;  
}
