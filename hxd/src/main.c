#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>
#include <inttypes.h>

typedef enum {
  true = 1,
  false = 0,
}bool;

typedef enum {
  notGiven = 0,
  dFlag = 1,
  hFlag = 2,
}flagOutput;

int printResult(flagOutput fOutput, bool flag64, char input[64]){
  
  int32_t num = (int32_t)strtoll(input, NULL, 0);
  
  switch (fOutput) {
    case dFlag:
      if(flag64){
        int64_t num64;
        num64 = (int64_t)strtoll(input, NULL, 0);
        printf("%" PRId64 "\n",num64);
        break;
      }
      printf("%" PRId32 "\n",num);
      break;
    case hFlag:
      if(flag64){
        int64_t num64;
        num64 = (int64_t)strtoll(input, NULL, 0);
        printf("0x%" PRIx64 "\n",num64);
        break;
      }
      printf("0x%" PRIx32 "\n",num);
      break;
    default:
      printf("Bad flag -d/-h\nUsage: hxd -d/-h [-64] num\n");
      return 1;
      break;
  }
  return 0;
}

bool checkInput(char input[64]) {
  int start = 0;
  if(input[start] == '-'){
    start++;
  }
  bool has_letter = false;
  
  
  for (int i = start; input[i] != '\0'; i++) {
      char c = input[i];

      // Controllo caratteri ammessi 0-9, a-f, A-F
      bool is_digit = isdigit(c);
      bool is_hex_letter = (tolower(c) >= 'a' && tolower(c) <= 'f');

      if(is_hex_letter == false && i == 1){
        if((c == 'x' || c == 'X') && input[0] == '0'){
          is_hex_letter = true;
        }
      }
      
      if (!is_digit && !is_hex_letter) {
          return false;
      }
      has_letter = has_letter || is_hex_letter;
  }
  if(has_letter && !(input[0] == '0' && tolower(input[1]) == 'x')){
      return false;
  }
  return true;
}

int main(int argc, char **argv){
  
  char input[64] = "";
  bool flag64 = false;
  flagOutput fOutput = notGiven;
    
  argc = argc - 1;
  if (argc < 1){
    printf("No arguments passed\nUsage: hxd -d/-h [-64] num\n");
    return 1;
  }
  argv = argv+1;

  for (int i = 0; i < argc; i++){
    if(strcmp(argv[i],"-d") == 0){
      fOutput = dFlag;
    }else if(strcmp(argv[i],"-h") == 0){
      fOutput = hFlag;
    }else if(strcmp(argv[i],"-64") == 0){
      flag64 = true;
    }else{
      strcpy(input, argv[i]);
    }
  }
  
  if (!checkInput(input)){
    printf("Invalid esadecimal number\n");
    return 1;
  }
    
  switch (fOutput) {
    case notGiven:
      fOutput = hFlag;
      for(int i = 0; input[i] != '\0'; i++){
        if(isalpha((unsigned char)input[i])){
          fOutput = dFlag;
          break;
        }
      }
      return printResult(fOutput, flag64, input);
      break;
    default:
      return printResult(fOutput, flag64, input);
      break;
  }

  return 0;
}
