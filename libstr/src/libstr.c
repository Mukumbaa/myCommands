#include "../include/libstr.h"

static size_t raw_strlen(const char *s) {
    if (!s) return 0;
    size_t len = 0;
    while (s[len] != '\0') {
        len++;
    }
    return len;
}

static void raw_copy(char *dest, const char *src, size_t n) {
    for (size_t i = 0; i < n; i++) {
        dest[i] = src[i];
    }
}

static void raw_move(char *dest, const char *src, size_t n) {
    if (dest == src || n == 0) return;

    if (dest < src) {
        for (size_t i = 0; i < n; i++) {
            dest[i] = src[i];
        }
    } else {
        for (size_t i = n; i > 0; i--) {
            dest[i - 1] = src[i - 1];
        }
    }
}

static int raw_compare(const char *b1, const char *b2, size_t n) {
    for (size_t i = 0; i < n; i++) {
        unsigned char c1 = (unsigned char)b1[i];
        unsigned char c2 = (unsigned char)b2[i];
        if (c1 != c2) {
            return (int)c1 - (int)c2;
        }
    }
    return 0;
}

static size_t get_next_capacity(size_t current, size_t needed) {
    if (needed > SIZE_MAX - 1) return 0; // it would go in overflow

    size_t cap = (current == 0) ? 16 : current;
    while (cap < needed) {
        // adding cap/2 would go in overflow
        if (cap > SIZE_MAX - (cap >> 1)) {
            cap = needed;
            break;
        }
        cap += (cap >> 1);
    }
    return cap;
}

//Init and destruction

String str_empty(void) {
    String s;
    s.capacity = 16;
    s.len = 0;
    s.str = malloc(s.capacity);
    if (s.str) {
        s.str[0] = '\0';
    } else {
        s.capacity = 0;
    }
    return s;
}

String str_init_with_cap(size_t initial_capacity) {
    String str;
    str.len = 0;
    str.capacity = (initial_capacity < 16) ? 16 : initial_capacity;
    str.str = malloc(str.capacity);
    if (str.str) {
        str.str[0] = '\0';
    } else {
        str.capacity = 0;
    }
    return str;
}

String str_init(const char *s) {
    if (!s) return str_empty();

    size_t actual_len = raw_strlen(s);
    String str;
    str.capacity = actual_len + 1;
    str.str = malloc(str.capacity);
    if (!str.str) {
        str.len = 0;
        str.capacity = 0;
        return str;
    }

    raw_copy(str.str, s, actual_len);
    str.str[actual_len] = '\0';
    str.len = actual_len;
    return str;
}

String str_from_fmt(const char *fmt, ...) {
    if (!fmt) return str_empty();

    va_list args;
    va_start(args, fmt);
    va_list args_copy;
    va_copy(args_copy, args);

    // determins the necessary length
    int written = vsnprintf(NULL, 0, fmt, args_copy);
    va_end(args_copy);

    if (written < 0) {
        va_end(args);
        return str_empty();
    }

    String s = str_init_with_cap((size_t)written + 1);
    if (s.str) {
        vsnprintf(s.str, s.capacity, fmt, args);
        s.len = (size_t)written;
    }
    va_end(args);
    return s;
}

void str_destroy(String *str) {
    if (!str) return;
    if (str->str) {
        free(str->str);
        str->str = NULL;
    }
    str->len = 0;
    str->capacity = 0;
}

void str_reset(String *str) {
    if (!str) return;
    if (str->str && str->capacity > 0) {
        str->str[0] = '\0';
        str->len = 0;
    } else {
        *str = str_empty();
    }
}

//Edit and concat

bool str_reserve(String *str, size_t min_capacity) {
    if (!str) return false;
    if (str->capacity >= min_capacity) return true;

    char *new_str = realloc(str->str, min_capacity);
    if (!new_str) return false;

    str->str = new_str;
    str->capacity = min_capacity;
    return true;
}

bool str_char(String *dest, char c) {
    if (!dest) return false;

    if (dest->len + 2 > dest->capacity) {
        size_t next_cap = get_next_capacity(dest->capacity, dest->len + 2);
        if (!str_reserve(dest, next_cap)) return false;
    }

    dest->str[dest->len] = c;
    dest->len++;
    dest->str[dest->len] = '\0';
    return true;
}

bool str_pop(String *dest, char *out_char) {
    if (!dest || dest->len == 0) return false;
    dest->len--;
    if (out_char) {
        *out_char = dest->str[dest->len];
    }
    dest->str[dest->len] = '\0';
    return true;
}

bool str_cat(String *dest, const String *src, size_t index) {
    if (!dest || !src || !src->str || index > dest->len) return false;
    if (src->len == 0) return true;

    // overflow check : dest->len + src->len
    if (SIZE_MAX - dest->len < src->len) return false;
    size_t total_len = dest->len + src->len;

    // overflow check for the null terminator: total_len + 1
    if (total_len == SIZE_MAX) return false;

    if (total_len + 1 > dest->capacity) {
        size_t next_cap = get_next_capacity(dest->capacity, total_len + 1);
        if (next_cap == 0 || !str_reserve(dest, next_cap)) return false;
    }

    if (index < dest->len) {
        raw_move(dest->str + index + src->len, dest->str + index, dest->len - index);
    }

    raw_copy(dest->str + index, src->str, src->len);

    dest->len = total_len;
    dest->str[dest->len] = '\0';
    return true;
}

bool str_append(String *dest, const String *src) {
    if (!dest) return false;
    return str_cat(dest, src, dest->len);
}

bool str_catr(String *dest, const char *src) {
    if (!dest || !src) return false;
    size_t slen = raw_strlen(src);
    String tmp = { .str = (char*)src, .len = slen, .capacity = slen + 1 };
    return str_cat(dest, &tmp, dest->len);
}

bool str_append_fmt(String *dest, const char *fmt, ...) {
    if (!dest || !fmt) return false;

    va_list args;
    va_start(args, fmt);
    va_list args_copy;
    va_copy(args_copy, args);

    int written = vsnprintf(NULL, 0, fmt, args_copy);
    va_end(args_copy);

    if (written < 0) {
        va_end(args);
        return false;
    }

    size_t needed_len = dest->len + (size_t)written;
    if (needed_len + 1 > dest->capacity) {
        size_t next_cap = get_next_capacity(dest->capacity, needed_len + 1);
        if (!str_reserve(dest, next_cap)) {
            va_end(args);
            return false;
        }
    }

    vsnprintf(dest->str + dest->len, dest->capacity - dest->len, fmt, args);
    va_end(args);

    dest->len = needed_len;
    dest->str[dest->len] = '\0';
    return true;
}

bool str_cpy(String *dest, const String *src) {
    if (!dest || !src) return false;
    if (dest == src) return true;

    if (src->len + 1 > dest->capacity) {
        if (!str_reserve(dest, src->len + 1)) return false;
    }

    raw_copy(dest->str, src->str, src->len);
    dest->len = src->len;
    dest->str[dest->len] = '\0';
    return true;
}

//Extract and remove

String str_substr(const String *s, size_t start, size_t count) {
    if (!s || !s->str || start >= s->len || count == 0) {
        return str_empty();
    }

    if (start + count > s->len) {
        count = s->len - start;
    }

    String sub = str_init_with_cap(count + 1);
    if (!sub.str) return str_empty();

    raw_copy(sub.str, s->str + start, count);
    sub.str[count] = '\0';
    sub.len = count;
    return sub;
}

bool str_remove(String *s, size_t index, size_t count) {
    if (!s || !s->str || index >= s->len || count == 0) return false;

    if (index + count >= s->len) {
        s->len = index;
        s->str[s->len] = '\0';
        return true;
    }

    size_t chars_after = s->len - (index + count);
    raw_move(s->str + index, s->str + index + count, chars_after);
    s->len -= count;
    s->str[s->len] = '\0';
    return true;
}

bool str_replace(String *s, const char *old_sub, const char *new_sub) {
    if (!s || !s->str || !old_sub || !new_sub) return false;
    size_t old_len = raw_strlen(old_sub);
    if (old_len == 0) return false;
    size_t new_len = raw_strlen(new_sub);

    String result = str_init_with_cap(s->capacity);
    if (!result.str) return false;

    String repl_token = { .str = (char*)new_sub, .len = new_len, .capacity = new_len + 1 };

    size_t i = 0;
    while (i < s->len) {
        if (i + old_len <= s->len && raw_compare(s->str + i, old_sub, old_len) == 0) {
            if (!str_cat(&result, &repl_token, result.len)) {
                str_destroy(&result);
                return false;
            }
            i += old_len;
        } else {
            if (!str_char(&result, s->str[i])) {
                str_destroy(&result);
                return false;
            }
            i++;
        }
    }

    str_destroy(s);
    *s = result;
    return true;
}

//cleanup and transformation

void str_ltrim(String *s) {
    if (!s || !s->str || s->len == 0) return;

    size_t start = 0;
    while (start < s->len && isspace((unsigned char)s->str[start])) {
        start++;
    }

    if (start == 0) return;

    size_t new_len = s->len - start;
    raw_move(s->str, s->str + start, new_len);
    s->len = new_len;
    s->str[s->len] = '\0';
}

void str_rtrim(String *s) {
    if (!s || !s->str || s->len == 0) return;

    size_t end = s->len;
    while (end > 0 && isspace((unsigned char)s->str[end - 1])) {
        end--;
    }

    s->len = end;
    s->str[s->len] = '\0';
}

void str_trim(String *s) {
    str_rtrim(s);
    str_ltrim(s);
}

void str_to_lower(String *s) {
    if (!s || !s->str) return;
    for (size_t i = 0; i < s->len; i++) {
        s->str[i] = (char)tolower((unsigned char)s->str[i]);
    }
}

void str_to_upper(String *s) {
    if (!s || !s->str) return;
    for (size_t i = 0; i < s->len; i++) {
        s->str[i] = (char)toupper((unsigned char)s->str[i]);
    }
}

void str_reverse(String *s) {
    if (!s || !s->str || s->len < 2) return;
    size_t left = 0;
    size_t right = s->len - 1;
    while (left < right) {
        char tmp = s->str[left];
        s->str[left] = s->str[right];
        s->str[right] = tmp;
        left++;
        right--;
    }
}

//cmp and query

int str_cmp(const String *str1, const String *str2) {
    if (!str1 || !str2) return (str1 == str2) ? 0 : (str1 ? 1 : -1);

    size_t min_l = (str1->len < str2->len) ? str1->len : str2->len;
    int res = raw_compare(str1->str, str2->str, min_l);
    if (res != 0) return res;

    if (str1->len < str2->len) return -1;
    if (str1->len > str2->len) return 1;
    return 0;
}

int str_cmpr(const String *str1, const char *str2) {
    if (!str1 || !str2) return (str1 && !str2) ? 1 : (!str1 && str2 ? -1 : 0);

    size_t i = 0;
    while (i < str1->len && str2[i] != '\0') {
        unsigned char c1 = (unsigned char)str1->str[i];
        unsigned char c2 = (unsigned char)str2[i];
        if (c1 != c2) {
            return (int)c1 - (int)c2;
        }
        i++;
    }

    if (i < str1->len) return 1;
    if (str2[i] != '\0') return -1;
    return 0;
}

bool str_equals(const String *str1, const String *str2) {
    if (!str1 || !str2) return false;
    if (str1->len != str2->len) return false;
    return raw_compare(str1->str, str2->str, str1->len) == 0;
}

bool str_equalsr(const String *str1, const char *str2) {
    if (!str1 || !str1->str || !str2) return false;

    size_t i = 0;
    while (i < str1->len && str2[i] != '\0') {
        if (str1->str[i] != str2[i]) {
            return false;
        }
        i++;
    }

    return (i == str1->len && str2[i] == '\0');
}

bool str_is_empty(const String *s) {
    return (!s || s->len == 0);
}

bool str_starts_with(const String *s, const char *prefix) {
    if (!s || !s->str || !prefix) return false;
    size_t i = 0;
    while (prefix[i] != '\0') {
        if (i >= s->len || s->str[i] != prefix[i]) {
            return false;
        }
        i++;
    }
    return true;
}

bool str_ends_with(const String *s, const char *suffix) {
    if (!s || !s->str || !suffix) return false;
    size_t suffix_len = raw_strlen(suffix);
    if (suffix_len > s->len) return false;

    size_t offset = s->len - suffix_len;
    for (size_t i = 0; i < suffix_len; i++) {
        if (s->str[offset + i] != suffix[i]) {
            return false;
        }
    }
    return true;
}

ptrdiff_t str_contains(const String *str, const String *pattern, int is_case_sensitive) {
    if (!str || !pattern || pattern->len == 0 || str->len < pattern->len) return -1;

    for (size_t i = 0; i <= str->len - pattern->len; i++) {
        size_t j = 0;
        while (j < pattern->len) {
            unsigned char c1 = (unsigned char)str->str[i + j];
            unsigned char c2 = (unsigned char)pattern->str[j];
            if (is_case_sensitive ? (c1 != c2) : (tolower(c1) != tolower(c2))) {
                break;
            }
            j++;
        }
        if (j == pattern->len) {
          // prevents overflow if the index access the maximum value rappresentable with sign 
          if (i > (size_t)PTRDIFF_MAX) return -1;
          return (ptrdiff_t)i;
        }
    }
    return -1;
}

ptrdiff_t str_containsr(const String *str, const char *pattern, int is_case_sensitive) {
    if (!str || !pattern) return -1;
    size_t plen = raw_strlen(pattern);
    String p = { .str = (char*)pattern, .len = plen, .capacity = plen + 1 };
    return str_contains(str, &p, is_case_sensitive);
}

ptrdiff_t str_find_last(const String *str, const char *pattern, int is_case_sensitive) {
    if (!str || !pattern || !str->str) return -1;
    size_t plen = raw_strlen(pattern);
    if (plen == 0 || str->len < plen) return -1;

    for (size_t i = str->len - plen + 1; i > 0; i--) {
        size_t idx = i - 1;
        size_t j = 0;
        while (j < plen) {
            unsigned char c1 = (unsigned char)str->str[idx + j];
            unsigned char c2 = (unsigned char)pattern[j];
            if (is_case_sensitive ? (c1 != c2) : (tolower(c1) != tolower(c2))) {
                break;
            }
            j++;
        }
        if (j == plen) {
            if (idx > (size_t)PTRDIFF_MAX) return -1;
            return (ptrdiff_t)idx;
        }
    }
    return -1;
}

//split and join

int str_split(String **arr, const String *str, char delimiter) {
  if (!arr || !str || !str->str) return -1;

  size_t capacity = 8;
  int count = 0;
  String *tokens = malloc(capacity * sizeof(String));
  if (!tokens) return -1;

  size_t start = 0;
  for (size_t i = 0; i <= str->len; i++) {
    if (i == str->len || str->str[i] == delimiter) {
      size_t token_len = i - start;

      if ((size_t)count >= capacity) {
        // checks that capacity * 2 does not go in overflow
        if (capacity > SIZE_MAX / 2) {
          return -1;
        }
        size_t new_cap = capacity * 2;

        // checks that new_cap * sizeof(String) does not go in overflow
        if (new_cap > SIZE_MAX / sizeof(String)) {
          return -1;
        }

        String *temp = realloc(tokens, new_cap * sizeof(String));
        if (!temp) { return -1; }
        tokens = temp;
        capacity = new_cap;
      }

      tokens[count] = str_init_with_cap(token_len + 1);
      raw_copy(tokens[count].str, str->str + start, token_len);
      tokens[count].str[token_len] = '\0';
      tokens[count].len = token_len;
      count++;

      start = i + 1;
    }
  }

  *arr = tokens;
  return count;
}

bool str_join(String *dest, const String *arr, int count, const char *separator) {
    if (!dest || !arr || count < 0) return false;
    str_reset(dest);
    if (count == 0) return true;

    for (int i = 0; i < count; i++) {
        if (!str_append(dest, &arr[i])) return false;
        if (separator && i < count - 1) {
            if (!str_catr(dest, separator)) return false;
        }
    }
    return true;
}

void str_free_split(String *arr, int count) {
    if (!arr) return;
    for (int i = 0; i < count; i++) {
        str_destroy(&arr[i]);
    }
    free(arr);
}
