/**
 * libstr.h
 * libstr is a library for c language to manage strings
*/
// DOCS:
// https://github.com/Mukumbaa/myCommands?tab=readme-ov-file#libstr
// 
#ifndef STR_LIB_H
#define STR_LIB_H
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

#define CASE_SENSITIVE 1
#define NO_CASE_SENSITIVE 0
typedef struct{
  char *str;
  int len;
}String;


String str_init(const char *s, int max_len);
void str_reset(String *str);
void str_resetr(char *str, int len);
void str_cat(String *dest, const String src, int index);
void str_catr(String *dest, const char *src, int max_len);
int str_cmp(String str1, String str2);
int str_cmpr(String str1, char *str2, int max_len);
int str_contains(String str, String pattern, int is_case_sensitive);
int str_containsr(String str, char *pattern, int max_len, int is_case_sensitive);
int str_split(String **arr, String str, char delimiter, int token_max_len);
int str_splitr(String **arr, char *str, char delimiter, int token_max_len, int max_len);
void str_cpy(String *dest, const String src);
// to do


#endif
