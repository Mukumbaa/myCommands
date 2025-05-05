#include <stdio.h>
#include "../include/strlib.h"

int main(){
  String string = str_init("Hello world!",25);
  String string2 = str_init("Hello people!",25);
  printf("\n\nTEST str_init()\n");
  printf("str:%s,len:%d\n",string.str,string.len);

  printf("\n\nTEST str_cat()\n");
  str_cat(&string, string);
  printf("str:%s,len%d\n", string.str,string.len);  
  string = str_init("Hello world!",25);
  str_cat_raw(&string, "Hello world!", 25);
  printf("str:%s,len:%d\n", string.str,string.len);

  printf("\n\nTEST str_cmp()\n");
  string = str_init("Hello world!",25);
  printf("cmp:%d:%s,%s\n", str_cmp(string,string),string.str,string.str);
  printf("cmp:%d:%s,%s\n", str_cmp(string,string2),string.str,string2.str);
  printf("cmp:%d:%s,%s\n", str_cmp_raw(string,"Hello world!", 25),string.str,"Hello world!");
  printf("cmp:%d:%s,%s\n", str_cmp_raw(string,"Hello world", 25),string.str,"Hello world");
  
  printf("\n\nTEST str_contains()\n");
  String pattern = str_init("world", 15);
  printf("contains:%d,str:%s,pattern:%s\n",str_contains(string, pattern, NO_CASE_SENSITIVE),string.str,pattern.str);
  printf("contains:%d,str:%s,pattern:%s\n",str_contains_raw(string, "world", 15, NO_CASE_SENSITIVE),string.str,"world");
      
  printf("\n\nTEST str_split()\n");
  string = str_init("Ciao a tutti quanti",50);
  String *arr;
  int dim = str_split(&arr, string, ' ', 15);
  printf("split:%d\n",dim);
  for(int i = 0; i < dim; i++){
    printf("token:%s,len:%d\n",arr[i].str,arr[i].len);
  }
  dim = str_split_raw(&arr, "Ciao a tutti quanti", ' ', 15, 25);
  printf("split:%d\n",dim);
  for(int i = 0; i < dim; i++){
    printf("token:%s,len:%d\n",arr[i].str,arr[i].len);
  }

  printf("\n\nTEST str_cpy()\n");
  String prova;
  str_cpy(&prova, string);
  printf("cpy:%s\n",prova.str);
  printf("original:%s\n",string.str);
  
  printf("\n\nTEST str_reset()\n");
  str_reset(&string);
  printf("str:%s,len:%d\n",string.str,string.len);
  return 0;
}
