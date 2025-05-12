#include <stdio.h>
#include "../include/libstr.h"
// void prova(char *str, int len){
//   for(int i = 0; i < len; i++){
//     str[i] = '\0';
//   }
// }

int main(){

  String str = str_init("ciao", 5);
  str_char(&str,'?');
  printf("%s-%d\n", str.str,str.len);
  return 0;
  // char str[] = "ciao";
  // char *token = malloc(4 * sizeof(char));
  
  // printf("%s\n",token);
  // prova(token, 5);
  // printf("-%s-\n",token);

  
//   String str1 = str_init("Ciao a tutti", 25);
//   String str2 = str_init("veramente ", 25);
//   str_cat(&str1, str2, -1);
//   printf("%s\n",str1.str);
//   String string = str_init("Hello world!",25);
//   String string2 = str_init("Hello people!",25);
//   printf("\n\nTEST str_init()\n");
//   printf("str:%s,len:%d\n",string.str,string.len);

//   printf("\n\nTEST str_cat()\n");
//   str_cat(&string, string, 5);
//   printf("str:%s,len%d\n", string.str,string.len);  
//   string = str_init("Hello world!",25);
//   str_catr(&string, "Hello world!", 25);
//   printf("str:%s,len:%d\n", string.str,string.len);

//   printf("\n\nTEST str_cmp()\n");
//   string = str_init("Hello world!",25);
//   printf("cmp:%d:%s,%s\n", str_cmp(string,string),string.str,string.str);
//   printf("cmp:%d:%s,%s\n", str_cmp(string,string2),string.str,string2.str);
//   printf("cmp:%d:%s,%s\n", str_cmpr(string,"Hello world!", 25),string.str,"Hello world!");
//   printf("cmp:%d:%s,%s\n", str_cmpr(string,"Hello world", 25),string.str,"Hello world");
  
//   printf("\n\nTEST str_contains()\n");
//   String pattern = str_init("world", 15);
//   printf("contains:%d,str:%s,pattern:%s\n",str_contains(string, pattern, NO_CASE_SENSITIVE),string.str,pattern.str);
//   printf("contains:%d,str:%s,pattern:%s\n",str_containsr(string, "world", 15, NO_CASE_SENSITIVE),string.str,"world");
      
//   printf("\n\nTEST str_split()\n");
//   string = str_init("Ciao a tutti quanti",50);
//   String *arr;
//   int dim = str_split(&arr, string, ' ', 15);
//   printf("split:%d\n",dim);
//   for(int i = 0; i < dim; i++){
//     printf("token:%s,len:%d\n",arr[i].str,arr[i].len);
//   }
//   dim = str_splitr(&arr, "Ciao a tutti quanti", ' ', 15, 25);
//   printf("split:%d\n",dim);
//   for(int i = 0; i < dim; i++){
//     printf("token:%s,len:%d\n",arr[i].str,arr[i].len);
//   }

//   printf("\n\nTEST str_cpy()\n");
//   String prova;
//   str_cpy(&prova, string);
//   printf("cpy:%s\n",prova.str);
//   printf("original:%s\n",string.str);
  
//   printf("\n\nTEST str_reset()\n");
//   str_reset(&string);
//   printf("str:%s,len:%d\n",string.str,string.len);
//   return 0;
}
