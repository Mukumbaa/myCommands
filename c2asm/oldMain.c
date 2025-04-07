#include <stdio.h>
#include <stdlib.h>
#include <process.h>

int main(int argc, char **argv){

  char *fileName = NULL;
  char *outputFileName = NULL;
  
  if(argc > 1 && argc <= 3){
    fileName = argv[1];
  }else{
    printf("Too few or too many arg");
    return 0;
  }
  if(argc == 3){
    outputFileName = argv[2];
    _execlp("gcc", "gcc", "-S", "-fno-asynchronous-unwind-tables", fileName, "-o", outputFileName, "-masm=intel", (char *)NULL, (char *)NULL);  
  }else{
    _execlp("gcc", "gcc", "-S", "-fno-asynchronous-unwind-tables", fileName, "-o", "output.asm", "-masm=intel", (char *)NULL);  
  }
  return 0;
}
