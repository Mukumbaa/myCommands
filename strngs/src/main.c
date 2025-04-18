#include <ctype.h>
#include <stdint.h>
#include <stdio.h>
// #include <stdlib.h>
#include <ctype.h>
#include <inttypes.h>
#include <stdlib.h>
#include <string.h>

#define MINL 4
#define LENGHT_FLAG "-l"
#define SEARCH_SENSITIVE_FLAG "-S"
#define SEARCH_NOSENSITIVE_FLAG "-s"



int pattern_match(char *input, char *pattern, int case_sensitive){

  int str_len = strlen(input);
  int str_len_p = strlen(pattern);
  int index = 0;
  
  for(int i = 0; i < str_len; i++){
    if(case_sensitive == 2 ? tolower(input[i]) == tolower(pattern[index]) : input[i] == pattern[index]) {
      // printf("%c %c\n", input[i],pattern[index]);
      if(index < str_len_p){
        index++;
      }
    }
    else if(index == str_len_p){
      return 0;
    }
    else{
      index = 0;
    }
  }
  if(index == str_len_p){
    return 0;
  }
  return 1;
}

int main(int argc, char **argv){
  // printf("%d\n",pattern_match("Ciao sono gabriele lippolis", "lippolis"));

  // argc--;
  if(argc < 2){
    printf("Expected argument(s)");
    return -1;
  }
  // argv++;
  int min_chars = 4;
  char input[64] = "\0";
  char search_for[1064] = "\0";
  int search_type = 0;
  
  for(int i = 1; i < argc; i++){
    if(strcmp(argv[i],LENGHT_FLAG) == 0){
      i++;
      min_chars = strtoll(argv[i], NULL, 0);
    }else if(strcmp(argv[i],SEARCH_SENSITIVE_FLAG) == 0){
      search_type = 1;
      i++;
      strcpy(search_for, argv[i]);
    }
    else if(strcmp(argv[i],SEARCH_NOSENSITIVE_FLAG) == 0){
      search_type = 2;
      i++;
      strcpy(search_for, argv[i]);
    }
    else{
      strcpy(input, argv[i]);
    }
  }

  FILE *file;
  int error = fopen_s(&file,input,"rb");
  if(error != 0) {
    printf("Error opening file: %d-%s\n",error,input);
    return -1;
  }


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
          if(search_type==0){
            printf("0x%" PRIx64 ": %s\n",string_start,buffer);
          }else{
            if(pattern_match(buffer, search_for,search_type) == 0){
              printf("0x%" PRIx64 ": %s\n",string_start,buffer);
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
  fclose(file);
  return 0;
}
