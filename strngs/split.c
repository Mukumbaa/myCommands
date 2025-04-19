#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int split(char ***list_string, char *string, char delimiter){

  int dim = 0;
  char token[64] = "\0";
  int index = 0;
  
  *list_string = NULL;

  for(int i = 0; string[i] != '\0'; i++){
    if(index > 64){
      printf("Token in split too big (>64)\n");
      return -1;
    }
    if(string[i] == '\\'){
      i++;
      token[index] = string[i];
      index++;
    }else if(string[i] == delimiter){
      if(index>0){
        dim++;
        *list_string = realloc(*list_string, dim * sizeof(char*));
        (*list_string)[dim - 1] = malloc(index+1);
        int j = 0;
        for(j = 0; j < index; j++){
          (*list_string)[dim - 1][j] = token[j];
        }
        (*list_string)[dim - 1][j] = '\0';
        index = 0;
        memset(token,0,64);
      }
    }else{
      token[index] = string[i];
      index++;
    }
  }
  if (index > 0) {
    dim++;
    *list_string = realloc(*list_string, dim * sizeof(char*));
    (*list_string)[dim - 1] = malloc(index + 1);
    for (int j = 0; j < index; j++) {
        (*list_string)[dim - 1][j] = token[j];
    }
    (*list_string)[dim - 1][index] = '\0';
  }
    
  return dim;
}



int main(int argc, char **argv){

  char **list_string = NULL;

  int dim = split(&list_string,argv[1],*argv[2]);

  for(int i = 0; i < dim; i++){
    printf("%s\n",list_string[i]);
  }

  return 0;
}
