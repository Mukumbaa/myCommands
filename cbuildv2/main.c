#include "cbuild.h"
#include <stdio.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>

void readCbuildFile(){

  
  CC = NULL;
  output = NULL;
  FLAG = NULL;
  LIB = NULL;
  FILES = NULL;
  memset(PARAM, 0, 256);
  numFlags = 0;
  numLibs = 0;
  numFiles = 0;

  FILE *file = fopen("cbuild","r");
  if(!file){
    printf("Error opening file\n");
    return;
  }

  char line[256];

  while(fgets(line, sizeof(line), file)){
    line[strcspn(line, "\n")] = '\0';

    
    char* separator = strchr(line, ':');
    if (separator == NULL) {
        printf("Invalid format: %s\n", line);
        continue;
    }

    *separator = '\0';
    char* key = line;
    char* value = separator + 1;
    value[strcspn(value, "\n")] = '\0';
    
    if (strcmp(key, "CC") == 0) {
      setCC("gcc");
    } else if (strcmp(key, "file") == 0) {
      addFile(value);
    } else if (strcmp(key, "flags") == 0) {
      addFlag(value);
    } else if (strcmp(key, "libs") == 0) {
      addLib(value);
    } else if (strcmp(key, "param") == 0) {
      strcpy(PARAM, value);
    } else if (strcmp(key, "output") == 0) {
      setOutputName(value);
    } else {
      printf("Invalid key: %s\n", key);
    }    
  }
}

int main(){

  char c;
  printf("Cbuilder running\n");
  
  while (1){
    printf("> ");
    // scanf(" %c",&c);
    c = getchar();
    while (getchar() != '\n');

    switch (c) {
      case 'b': // build
        build();
        break;
      case 'r': // run program
        run();
        break;
      case 'p': // run program
        param();
        run();
        break;
      case 'P': // run program
        build();
        param();
        run();
        break;
      case 'a': // all (u c r)
        readCbuildFile();
        build();
        run();
        break;
      case 'l': // list from file
        recap();
        break;
      case 'u': // read cbuild file
        readCbuildFile();
        recap();
        break;
      case 'c': // read cbuild file
        system("cls");
        break;
      case 'q':
        printf("Exiting Cbuilder.\n");
        return 0;
      default:
        break;
    }
  }
}
