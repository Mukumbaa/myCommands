#include <ctype.h>
#include <stdint.h>
#include <stdio.h>
#include <ctype.h>
#include <inttypes.h>
#include <stdlib.h>
#include <string.h>

#define MINL 4
#define LENGHT_FLAG "-l"
#define SEARCH_SENSITIVE_FLAG "-S"
#define SEARCH_NOSENSITIVE_FLAG "-s"
#define SENSITIVE 1
#define NOSENSITIVE 2
#define NOSEARCH 0

typedef enum bool{
  false = 0,
  true = 1,
}bool;

bool pattern_match(char *input, char *pattern, int case_sensitive){

  int str_len = strlen(input);
  int str_len_p = strlen(pattern);
  int index = 0;
  
  for(int i = 0; i < str_len; i++){
    if(case_sensitive == NOSENSITIVE ? tolower(input[i]) == tolower(pattern[index]) : input[i] == pattern[index]) {
      if(index < str_len_p){
        index++;
      }
    }
    else if(index == str_len_p){
      return true;
    }
    else{
      index = 0;
    }
  }
  if(index == str_len_p){
    return true;
  }
  return false;
}
bool multi_pattern_match(char *input, char **patterns, int dim, int case_sensitive){

  bool result = false;

  for(int i = 0; i < dim; i++){
    result = result || pattern_match(input, patterns[i],case_sensitive);
  }
  return result;
}
int split(char ***list_string, char *string){
  int dim = 0;
  for(int i = 0; string[i] != '\0'; i++){
    if(string[i] == ';'){
      dim++;
    }
  }
  *list_string = malloc(dim * sizeof(char*));
  if(*list_string == NULL){
    printf("Allocation error\n");
    return -1;
  }
  dim++;

  char *token = strtok(string, ";");
  int i = 0;
  // printf("%s\n",token);
  while(token != NULL){
    (*list_string)[i] = malloc(strlen(token)+1);
    if((*list_string)[i] == NULL){
      printf("Allocation error\n");
      return -1;
    }
    strcpy((*list_string)[i],token);
    i++;
    token = strtok(NULL,";");
  }
  return dim;
}

void print_ascii_strings(FILE *file, int min_chars, int search_type, int dim, char **pattern){

  char buffer[1064] = "\0";
  int c;
  int64_t index = 0;
  int64_t offset = 0;
  int64_t string_start = 0;

  while((c = fgetc(file)) != EOF){
    // break;
    if(isprint(c)){
      if(index == 0){
        string_start = offset;
      }
      if(index < 1064 - 1){
        buffer[index++] = (char)c;
      }
    }else{
      if(index >= min_chars){
        buffer[index] = '\0';
        if(buffer[0] != '.'){
          if(search_type==NOSEARCH){
            printf("[ASCII] 0x%" PRIx64 ": %s\n",string_start,buffer);
          }else{
            if(multi_pattern_match(buffer, pattern, dim, search_type) == true){
            // if(pattern_match(buffer, search_for,search_type) == 0){
              printf("[ASCII] 0x%" PRIx64 ": %s\n",string_start,buffer);
            }
          }
        }
      }
      index = 0;
    }
    offset++;
  }

  if(index >= min_chars){
    buffer[index] = '\0';
    printf("0x%" PRIx64 ": %s\n",string_start,buffer);
  }
  return;  
}

int main(int argc, char **argv){
  if(argc < 2){
    printf("Expected argument(s)");
    return -1;
  }
  int min_chars = 4;
  char input[64] = "\0";
  char search_for[1064] = "\0";
  int search_type = NOSEARCH;
  
  for(int i = 1; i < argc; i++){
    if(strcmp(argv[i],LENGHT_FLAG) == 0){
      i++;
      min_chars = strtoll(argv[i], NULL, 0);
    }else if(strcmp(argv[i],SEARCH_NOSENSITIVE_FLAG) == 0){
      search_type = NOSENSITIVE;
      i++;
      strcpy(search_for, argv[i]);
    }
    else if(strcmp(argv[i],SEARCH_SENSITIVE_FLAG) == 0){
      search_type = SENSITIVE;
      i++;
      strcpy(search_for, argv[i]);
    }
    else{
      strcpy(input, argv[i]);
    }
  }

  FILE *file = NULL;
  int error = fopen_s(&file,input,"rb");
  if(error != 0) {
    printf("Error opening file: %d-%s\n",error,input);
    return -1;
  }

  
  char **pattern = NULL;
  int dim = 0;

  if(search_type != NOSEARCH){
    dim = split(&pattern, search_for);
  }
  print_ascii_strings(file, min_chars, search_type, dim, pattern);
  fclose(file);
  if(search_type != NOSEARCH){
    // Free memory
    for (int i = 0; i < dim; i++) {
        free(pattern[i]);
    }
    free(pattern);
  }
  return 0;
}
