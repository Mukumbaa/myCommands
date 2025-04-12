#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>
#include <inttypes.h>

typedef enum bool{
  true = 1,
  false = 0,
}bool;

typedef enum flagOutput{
  notGiven = 0,
  dFlag = 1,
  hFlag = 2,
}flagOutput;

int printResult(flagOutput fOutput, bool flag64, char input[64]){
  switch (fOutput) {
    case dFlag:
      if(!flag64){
        int32_t num;
        num = (int32_t)strtoll(input, NULL, 0);
        printf("%" PRId32 "\n",num);
      }else{
        int64_t num;
        num = (int64_t)strtoll(input, NULL, 0);
        printf("%" PRId64 "\n",num);
      }
      break;
    case hFlag:
      if(!flag64){
        int32_t num;
        num = (int32_t)strtoll(input, NULL, 0);
        printf("0x%" PRIx32 "\n",num);
      }else{
        int64_t num;
        num = (int64_t)strtoll(input, NULL, 0);
        printf("0x%" PRIx64 "\n",num);
      }
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
  for (int i = start; input[i] != '\0'; i++) {
      char c = input[i];

      // Controllo caratteri ammessi: 0-9, a-f, A-F
      bool is_hex_digit = (isdigit(c) || (tolower(c) >= 'a' && tolower(c) <= 'f'));
      bool is_allowed_x = (i == 1 && tolower(c) == 'x' && input[0] == '0');

      if (!is_hex_digit && !is_allowed_x) {
          return false;  // Carattere non valido
      }
  }
  return true;  // Stringa valida
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
      strcpy_s(input,64, argv[i]);
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
