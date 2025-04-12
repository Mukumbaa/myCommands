#include <stdio.h>
#include <stdlib.h>
#include <process.h>
#include <string.h>

static inline void execCommand(int eOk, int edOk, char *file1, char *file2){

  // char file2Ext[64] = "";
  char command[256];
  
  // .asm
  // sprintf(file2Ext, "%s.asm", file2);
  sprintf(command, "gcc -S -fno-asynchronous-unwind-tables %s -o %s.asm -masm=intel", file1, file2);
  if (system(command) != 0) {
      perror("Error in .asm");
      return;
  }

  if (eOk) {
    
    // binary
    sprintf(command, "gcc %s -o %s.exe", file1, file2);
    if (system(command) != 0) {
        perror("error in .exe");
        return;
    }

    // disassembly
    // memset(file2Ext,0,sizeof(file2Ext));
    // sprintf(file2Ext, "%s.txt", file2);

    #ifdef _WIN32
      // Windows
      sprintf(command, "objdump -d -M intel %s.exe > %s.txt", file2, file2);
    #else
      // Unix
      sprintf(command, "objdump -d -M intel %s > %s.txt", file2, file2);
    #endif

    if (system(command) != 0) {
        perror("Error in .txt");
        return;
    }  
    if (edOk){
      sprintf(command, "del %s.exe", file2);
      if (system(command) != 0){
        perror("Error in edOk");
        return;
      }
    }

  }
  return;
}

int main(int argc, char **argv){
  int eOk = 0;
  int edOk = 0;
  char *file1 = NULL;
  char *file2 = NULL;
  
  if (argc < 2){
    fprintf(stderr,"Usage: c2asm [-e] file1 [file2]\n");
    return 1;
  }

  int argIndex = 1;

  // Checking if -e flag is given
  if (strcmp(argv[argIndex],"-e") == 0){
    eOk = 1;
    argIndex++;
  }else if (strcmp(argv[argIndex],"-ed") == 0){
    eOk = 1;
    edOk = 1;
    argIndex++;
  }

  if (argc - argIndex < 1 || argc - argIndex > 2){
    fprintf(stderr,"Incorrect number of files\n");
    fprintf(stderr,"Usage: c2asm [-e] file1 [file2]\n");
    return 1;
  }

  // Taking file names
  file1 = argv[argIndex];
  if (argc - argIndex == 2){
    if (strstr(argv[argIndex+1],".")){
      fprintf(stderr, "Name of second file needs to be given without the extension\n");
      fprintf(stderr,"Usage: c2asm [-e] file1 [file2]\n");
      return 1;
    }else{
      file2 = argv[argIndex+1];
    }
  }else{
    file2 = "output";
  }

  execCommand(eOk, edOk, file1, file2);
}
