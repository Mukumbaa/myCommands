# `libstr` — Safe Dynamic String Library for C

`libstr` is a memory-safe, cross-platform dynamic string library written in pure C99. It is designed as a modern replacement for standard C-style strings and `<string.h>` operations.

---

## Table of Contents
1. [Key Features](#key-features)
2. [Data Structure](#data-structure)
3. [Building and Linking](#building-and-linking)
4. [Memory Ownership and Safety Guidelines](#memory-ownership-and-safety-guidelines)
5. [API Reference](#api-reference)
   - [Creation and Destruction](#1-creation-and-destruction)
   - [Modification and Concatenation](#2-modification-and-concatenation)
   - [Extraction and Removal](#3-extraction-and-removal)
   - [Cleaning and Transformation](#4-cleaning-and-transformation)
   - [Comparison and Predicates](#5-comparison-and-predicates)
   - [Search and Find](#6-search-and-find)
   - [Tokenization and Joining](#7-tokenization-and-joining)
6. [Complete Code Example](#complete-code-example)

---

## Key Features

- **No `<string.h>` Dependency**: All string manipulations, byte copies, comparisons, and memory shifts are implemented using custom internal routines.
- **Buffer Overflow Protection**: Tracks buffer capacity explicitly and checks for integer overflow before every allocation or math operation.
- **Amortized Geometric Growth**: Minimizes heap fragmentation by reallocating buffers at a $1.5\times$ growth factor.
- **Safe Return Values**: Allocating functions return `bool` (`true` on success, `false` on OOM or invalid parameters). Search functions return `ptrdiff_t` to avoid 32/64-bit integer truncation issues.
- **Null-Terminator Guarantee**: Every valid `String` is kept null-terminated (`\0`), making it compatible with existing C APIs (`printf("%s", s.str)`).

---

## Data Structure

```c
typedef struct {
    char *str;        /* Pointer to heap-allocated, null-terminated buffer */
    size_t len;       /* Number of characters (excluding '\0') */
    size_t capacity;  /* Total allocated size of the buffer */
} String;
```

---

## Building and Linking

### Using `make`
To build the static library and package it into the `ship/` folder:
```bash
make lib
```
This produces:
- `ship/libstr.h`
- `ship/libstr_linux.a` (or `libstr_win.a` on Windows)

### Running Unit Tests
```bash
make test
```

### Linking in Your Project
```bash
gcc -std=c99 -Wall -Wextra -Iinclude main.c ship/libstr_linux.a -o my_app
```

---

## Memory Ownership and Safety Guidelines

1. **Always clean up**: Every `String` initialized with `str_init()`, `str_empty()`, `str_init_with_cap()`, or `str_from_fmt()` must eventually be freed with `str_destroy(&s)`.
2. **Double-destroy is safe**: Calling `str_destroy(&s)` sets `s.str` to `NULL`, `s.len` to `0`, and `s.capacity` to `0`. Passing `NULL` to `str_destroy(NULL)` is a no-op.
3. **Pointers for mutations**: Pass `String *` when a function mutates the string, and `const String *` when the function only reads data.

---

## API Reference

### 1. Creation and Destruction

#### `String str_init(const char *s)`
Initializes a `String` by copying a null-terminated C string. If `s` is `NULL`, creates an empty string.
```c
String s = str_init("Hello World");
/* s.len == 11, s.capacity >= 12 */
str_destroy(&s);
```

#### `String str_init_with_cap(size_t initial_capacity)`
Allocates an empty string with a pre-allocated capacity to avoid reallocations when appending data later.
```c
String s = str_init_with_cap(1024);
/* Ready to receive up to 1023 characters without reallocating */
str_destroy(&s);
```

#### `String str_empty(void)`
Constructs an empty string with a default capacity of 16 bytes.
```c
String s = str_empty();
/* s.len == 0, s.str[0] == '\0' */
str_destroy(&s);
```

#### `String str_from_fmt(const char *fmt, ...)`
Creates a new string from a `printf`-style format string. Automatically measures the required size and allocates the exact buffer needed.
```c
String s = str_from_fmt("User %s has ID #%04d", "Alice", 42);
/* s.str contains "User Alice has ID #0042" */
str_destroy(&s);
```

#### `void str_destroy(String *str)`
Frees the heap buffer and resets all fields to 0/NULL.
```c
str_destroy(&s);
assert(s.str == NULL && s.len == 0 && s.capacity == 0);
```

#### `void str_reset(String *str)`
Clears the contents of the string to length 0 without freeing the allocated buffer. Reuses the capacity for future operations.
```c
str_reset(&s);
/* s.len == 0, s.capacity is preserved */
```

---

### 2. Modification and Concatenation

#### `bool str_reserve(String *str, size_t min_capacity)`
Ensures that the string has a capacity of at least `min_capacity` bytes. Returns `false` on allocation failure.

#### `bool str_char(String *dest, char c)`
Appends a single character to the end of the string. Automatically grows the capacity if necessary.
```c
String s = str_empty();
str_char(&s, 'A');
str_char(&s, 'B');
/* s.str == "AB" */
str_destroy(&s);
```

#### `bool str_pop(String *dest, char *out_char)`
Removes the last character from the string. If `out_char` is non-NULL, stores the removed character. Returns `false` if the string is empty.
```c
char c;
str_pop(&s, &c);
```

#### `bool str_cat(String *dest, const String *src, size_t index)`
Inserts `src` into `dest` at byte position `index`. Elements after `index` are shifted to make room.
```c
String dest = str_init("ac");
String src  = str_init("b");
str_cat(&dest, &src, 1);
/* dest.str == "abc" */
str_destroy(&src);
str_destroy(&dest);
```

#### `bool str_append(String *dest, const String *src)`
Appends `src` to the end of `dest`. Equivalent to `str_cat(dest, src, dest->len)`.

#### `bool str_catr(String *dest, const char *src)`
Appends a raw, null-terminated C string to `dest`.
```c
String s = str_init("Hello");
str_catr(&s, " World!");
/* s.str == "Hello World!" */
str_destroy(&s);
```

#### `bool str_append_fmt(String *dest, const char *fmt, ...)`
Appends a formatted string to `dest` safely, resizing the buffer as needed.
```c
String s = str_init("Total: ");
str_append_fmt(&s, "%d EUR (VAT %.1f%%)", 100, 22.0);
/* s.str == "Total: 100 EUR (VAT 22.0%)" */
str_destroy(&s);
```

#### `bool str_cpy(String *dest, const String *src)`
Deep-copies `src` into `dest`. Reuses existing capacity in `dest` if sufficient; handles self-copy safely.
```c
String a = str_init("Alpha");
String b = str_empty();
str_cpy(&b, &a);
/* b is an independent copy of a */
```

---

### 3. Extraction and Removal

#### `String str_substr(const String *s, size_t start, size_t count)`
Extracts a substring starting at index `start` with a maximum length of `count`. Returns a new, independent `String`.
```c
String s = str_init("0123456789");
String sub = str_substr(&s, 3, 4);
/* sub.str == "3456", sub.len == 4 */
str_destroy(&sub);
str_destroy(&s);
```

#### `bool str_remove(String *s, size_t index, size_t count)`
Deletes `count` characters starting from `index` and shifts the remaining characters to the left in-place.
```c
String s = str_init("Hello Beautiful World");
str_remove(&s, 5, 10);
/* s.str == "Hello World" */
str_destroy(&s);
```

#### `bool str_replace(String *s, const char *old_sub, const char *new_sub)`
Replaces all occurrences of `old_sub` with `new_sub`.
```c
String s = str_init("red green red blue red");
str_replace(&s, "red", "yellow");
/* s.str == "yellow green yellow blue yellow" */
str_destroy(&s);
```

---

### 4. Cleaning and Transformation

#### `void str_ltrim(String *s)` / `void str_rtrim(String *s)` / `void str_trim(String *s)`
Strips leading, trailing, or both sides of whitespace (`' '`, `'\t'`, `'\n'`, `'\r'`) in-place.
```c
String s = str_init("   \t  clean me \n ");
str_trim(&s);
/* s.str == "clean me" */
str_destroy(&s);
```

#### `void str_to_lower(String *s)` / `void str_to_upper(String *s)`
Converts the string to lowercase or uppercase in-place.
```c
String s = str_init("Hello World!");
str_to_upper(&s); /* "HELLO WORLD!" */
str_to_lower(&s); /* "hello world!" */
str_destroy(&s);
```

#### `void str_reverse(String *s)`
Reverses the characters of the string in-place.
```c
String s = str_init("stressed");
str_reverse(&s);
/* s.str == "desserts" */
str_destroy(&s);
```

---

### 5. Comparison and Predicates

#### `int str_cmp(const String *str1, const String *str2)`
Lexicographically compares two `String` structs:
- `< 0` if `str1 < str2`
- `0` if `str1 == str2`
- `> 0` if `str1 > str2`

#### `int str_cmpr(const String *str1, const char *str2)`
Compares a `String` with a null-terminated C string.

#### `bool str_equals(const String *str1, const String *str2)`
Optimized equality check. First compares lengths (instant $O(1)$ fail on mismatch), then compares character content.

#### `bool str_is_empty(const String *s)`
Returns `true` if `s` is `NULL` or `s->len == 0`.

#### `bool str_starts_with(const String *s, const char *prefix)`
Checks whether `s` begins with `prefix`.

#### `bool str_ends_with(const String *s, const char *suffix)`
Checks whether `s` ends with `suffix`.

---

### 6. Search and Find

Constants for case sensitivity:
- `CASE_SENSITIVE` (`1`)
- `NO_CASE_SENSITIVE` (`0`)

#### `ptrdiff_t str_contains(const String *str, const String *pattern, int is_case_sensitive)`
#### `ptrdiff_t str_containsr(const String *str, const char *pattern, int is_case_sensitive)`
Finds the **first** occurrence of `pattern` inside `str`.
- Returns the zero-based index of the first matching character.
- Returns `-1` if not found.

```c
String hay = str_init("Database Server");
ptrdiff_t pos = str_containsr(&hay, "server", NO_CASE_SENSITIVE);
/* pos == 9 */
str_destroy(&hay);
```

#### `ptrdiff_t str_find_last(const String *str, const char *pattern, int is_case_sensitive)`
Finds the **last** occurrence of `pattern` inside `str`. Ideal for parsing file extensions or directory paths.
```c
String path = str_init("/var/log/nginx/access.log");
ptrdiff_t idx = str_find_last(&path, "/", CASE_SENSITIVE);
/* idx == 14 (position of the last slash) */
str_destroy(&path);
```

---

### 7. Tokenization and Joining

#### `int str_split(String **arr, const String *str, char delimiter)`
Splits `str` by `delimiter` and allocates a dynamic array of `String` tokens.
- Handles empty tokens between consecutive delimiters (e.g., `a,,b` yields 3 tokens).
- Returns the number of tokens created, or `-1` on error.
- **Must be freed using `str_free_split()`**.

```c
String csv = str_init("admin,user,moderator");
String *tokens = NULL;

int count = str_split(&tokens, &csv, ',');
/* count == 3 */
/* tokens[0] -> "admin", tokens[1] -> "user", tokens[2] -> "moderator" */

str_free_split(tokens, count);
str_destroy(&csv);
```

#### `bool str_join(String *dest, const String *arr, int count, const char *separator)`
Joins an array of `String` structures using `separator` into `dest`.
```c
String words[3] = { str_init("Linux"), str_init("Windows"), str_init("macOS") };
String joined = str_empty();

str_join(&joined, words, 3, " | ");
/* joined.str == "Linux | Windows | macOS" */

for (int i = 0; i < 3; i++) str_destroy(&words[i]);
str_destroy(&joined);
```

#### `void str_free_split(String *arr, int count)`
Frees each individual `String` in the array and the array pointer itself.

---

## Complete Code Example

Save this snippet as `example.c` to see how the API works end-to-end:

```c
#include <stdio.h>
#include "libstr.h"

int main(void) {
    // 1. Creation and formatted creation
    String greeting = str_from_fmt("   Hello, %s! Welcome to version %d.%d.   ", "Developer", 2, 0);

    // 2. In-place manipulation
    str_trim(&greeting);
    printf("Trimmed: \"%s\"\n", greeting.str);

    // 3. Search and replace
    if (str_starts_with(&greeting, "Hello")) {
        str_replace(&greeting, "Developer", "Engineer");
    }
    printf("Replaced: \"%s\"\n", greeting.str);

    // 4. Tokenization (split & join)
    String *tokens = NULL;
    int count = str_split(&tokens, &greeting, ' ');

    printf("\nTokens found (%d):\n", count);
    for (int i = 0; i < count; i++) {
        printf(" [%d] %s (len: %zu)\n", i, tokens[i].str, tokens[i].len);
    }

    // 5. Joining tokens back together
    String serialized = str_empty();
    str_join(&serialized, tokens, count, " :: ");
    printf("\nSerialized: \"%s\"\n", serialized.str);

    // 6. Cleanup (Memory leak prevention)
    str_free_split(tokens, count);
    str_destroy(&serialized);
    str_destroy(&greeting);

    return 0;
}
```
