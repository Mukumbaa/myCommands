#include <stdio.h>
#include <stdlib.h>
#include <process.h>

int main(int argc, char **argv){
  char *fileName = argv[1];
  // printf("%s\n",fileName);
  execlp("gcc", "gcc", "-S", "-fno-asynchronous-unwind-tables", fileName, "-o", "output.asm", "-masm=intel", (char *)NULL);  
  return 0;
}
