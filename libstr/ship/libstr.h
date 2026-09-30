#ifndef STR_LIB_H
#define STR_LIB_H

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdarg.h>
#include <stdio.h>
#include <ctype.h>

#define CASE_SENSITIVE 1
#define NO_CASE_SENSITIVE 0

typedef struct {
    char *str;
    size_t len;
    size_t capacity;
} String;

/* ========================================================================= */
/*                          CREAZIONE E DISTRUZIONE                          */
/* ========================================================================= */
String str_init(const char *s);
String str_init_with_cap(size_t initial_capacity);
String str_empty(void);
String str_from_fmt(const char *fmt, ...);
void   str_destroy(String *str);
void   str_reset(String *str);

/* ========================================================================= */
/*                        MODIFICA E CONCATENAZIONE                          */
/* ========================================================================= */
bool   str_reserve(String *str, size_t min_capacity);
bool   str_cat(String *dest, const String *src, size_t index);
bool   str_catr(String *dest, const char *src);
bool   str_append(String *dest, const String *src);
bool   str_append_fmt(String *dest, const char *fmt, ...);
bool   str_char(String *dest, char c);
bool   str_pop(String *dest, char *out_char);
bool   str_cpy(String *dest, const String *src);

/* ========================================================================= */
/*                         ESTRAZIONE E RIMOZIONE                            */
/* ========================================================================= */
String str_substr(const String *s, size_t start, size_t count);
bool   str_remove(String *s, size_t index, size_t count);
bool   str_replace(String *s, const char *old_sub, const char *new_sub);

/* ========================================================================= */
/*                        PULIZIA E TRASFORMAZIONE                           */
/* ========================================================================= */
void   str_trim(String *s);
void   str_ltrim(String *s);
void   str_rtrim(String *s);
void   str_to_lower(String *s);
void   str_to_upper(String *s);
void   str_reverse(String *s);

/* ========================================================================= */
/*                           CONFRONTO E QUERY                               */
/* ========================================================================= */
int       str_cmp(const String *str1, const String *str2);
int       str_cmpr(const String *str1, const char *str2);
bool      str_equals(const String *str1, const String *str2);
bool      str_is_empty(const String *s);
bool      str_starts_with(const String *s, const char *prefix);
bool      str_ends_with(const String *s, const char *suffix);
ptrdiff_t str_contains(const String *str, const String *pattern, int is_case_sensitive);
ptrdiff_t str_containsr(const String *str, const char *pattern, int is_case_sensitive);
ptrdiff_t str_find_last(const String *str, const char *pattern, int is_case_sensitive);

/* ========================================================================= */
/*                           SPLIT E JOIN                                    */
/* ========================================================================= */
int    str_split(String **arr, const String *str, char delimiter);
bool   str_join(String *dest, const String *arr, int count, const char *separator);
void   str_free_split(String *arr, int count);

#endif /* STR_LIB_H */
