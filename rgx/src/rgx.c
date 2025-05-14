#include "../include/rgx.h"
#include <complex.h>

bool match_here(char c, regex_e re) {
  // printf("HERE-%c-%s\n",c,re.str.str);

  if (re.r_op == dot) {
    return c != '\0'; // accetta qualsiasi carattere tranne fine stringa
  }
  for (int i = 0; i < re.str.len; i++) {
    if (c == re.str.str[i]) {
      // printf("TRUE\n");
      return true;
    }
  }
  // printf("FALSE\n");
  return false;
}
bool match_regex(regex_e *regex, int size, String input, int *s_idx) {

  int r_idx = 0;
  (*s_idx) = 0;
  int n_loop = 0;

  while (r_idx < size) {
    n_loop++;

    regex_e curr = regex[r_idx];
    action act = one;
    r_idx++;

    if (r_idx < size) {

      switch (regex[r_idx].r_op) {
      case star:
        act = zeroOrMore;
        r_idx++;
        break;
      case question:
        act = zeroOrOne;
        r_idx++;
        break;
      case plus:
        act = oneOrMore;
        r_idx++;
        break;
      case alter:
        act = one;
        r_idx++;
        break;
      default:
        act = one;
        break;
      }
    }
    int match = 0;
    if (act == one) {
      if ((*s_idx) >= input.len)
        return false;
      if (match_here(input.str[(*s_idx)], curr)) {
        (*s_idx)++;
      } else {
        return false;
      }
    } else if (act == zeroOrOne) {
      if ((*s_idx) < input.len && match_here(input.str[(*s_idx)], curr)) {
        (*s_idx)++;
      }
      // altrimenti, va bene anche 0 match
    } else if (act == zeroOrMore) {

      bool has_next = r_idx < size;
      regex_e next;
      if (has_next)
        next = regex[r_idx];

      if (curr.r_op == dot) {
        if (has_next == false)
          return true;

        while ((*s_idx) < input.len && match_here(input.str[(*s_idx)], curr)) {
          if (match_here(input.str[(*s_idx)], next)) {
            break;
          }
          (*s_idx)++;
        }

      } else {
        while ((*s_idx) < input.len && match_here(input.str[(*s_idx)], curr)) {
          (*s_idx)++;
        }
      }

    } else if (act == oneOrMore) {
      if ((*s_idx) >= input.len || !match_here(input.str[(*s_idx)], curr)) {
        return false;
      }
      (*s_idx)++;
      while ((*s_idx) < input.len && match_here(input.str[(*s_idx)], curr)) {
        (*s_idx)++;
      }
    }
  }
  // return s_idx == input.len;
  return true;
}
bool match_regex_anywhere(regex_e *regex, int size, String input) {
  int start = 0;
  int count = 0;
  int s_idx = 0;

  if (regex[0].r_op == start_str) {
    if (match_regex(&regex[1], size - 1, input, &s_idx) /*&& s_idx > 0*/) {

      // printf("Match\n");
      printf("start:%d-s_idx:%d\n", start, s_idx);
      printf("Match: %s\n", input.str);
      printf("       ");
      if (s_idx == 0) {
        for (int i = 0; i < input.len; i++) {
          printf("~");
        }
        printf("\n");
        return true;
      }
      for (int i = 0; i < start; i++)
        printf(" ");
      for (int i = 0; i < s_idx; i++)
        printf("~");
      printf("\n");
      return true;
    } else {
      printf("No match\n");
      return false;
    }
  }

  while (start < input.len) {
    s_idx = 0;
    String sliced = {.str = &input.str[start], .len = input.len - start};

    if (match_regex(regex, size, sliced, &s_idx) /*&& s_idx > 0*/) {
      count++;
      printf("start:%d-s_idx:%d\n", start, s_idx);
      printf("Match: %s\n", input.str);
      printf("       ");
      if (s_idx == 0) {
        for (int i = 0; i < input.len; i++) {
          printf("~");
        }
        printf("\n");
        // return true;
      } else {
        for (int i = 0; i < start; i++)
          printf(" ");
        for (int i = 0; i < s_idx; i++)
          printf("~");
        printf("\n");
      }

      if (s_idx == 0) {
        s_idx++;
      }
      start += s_idx; // Avanza fino alla fine del match
    } else {
      start++; // Nessun match qui, avanza di uno
    }
  }

  if (count != 0) {
    return true;
  }

  printf("No match\n");
  return false;
}

int parse_regex(String regex, regex_e **list, int *size) {
  String buffer;
  int len = 0;
  regex_op r_op;
  bool flag = false;
  int old_i = 0;

  for (int i = 0; i < regex.len; i++) {
    str_reset(&buffer);
    len = 0;
    switch (regex.str[i]) {
    case '^':
      len++;
      str_char(&buffer, '^');
      r_op = start_str;
      break;
    case '$':
      len++;
      str_char(&buffer, '$');
      r_op = end_str;
      break;
    case '.':
      len++;
      str_char(&buffer, '.');
      r_op = dot;
      break;
    case '*':
      len++;
      str_char(&buffer, '*');
      r_op = star;
      break;
    case '+':
      len++;
      str_char(&buffer, '+');
      r_op = plus;
      break;
    case '?':
      len++;
      str_char(&buffer, '?');
      r_op = question;
      break;
    case '|':
      len++;
      str_char(&buffer, '|');
      r_op = alter;
    case '\\':
      len++;
      str_char(&buffer, '\\');
      r_op = escape;
      break;
    case '[':
      len++;
      flag = false;
      old_i = i;
      str_char(&buffer, regex.str[i]);
      for (i = i + 1; i < regex.len && flag == false; i++) {
        if (regex.str[i] == ']')
          flag = true;
        len++;
        str_char(&buffer, regex.str[i]);
      }
      if (flag == false) {
        printf("Not closing set at index: %d\n%s\n", old_i, regex.str);
        for (i = 0; i < old_i; i++) {
          printf(" ");
        }
        printf("^\n");
        return -1;
      }
      i--;
      r_op = set;
      break;
    case '(':
      len++;
      flag = false;
      old_i = i;
      str_char(&buffer, regex.str[i]);
      for (i = i + 1; i < regex.len && flag == false; i++) {
        if (regex.str[i] == ')')
          flag = true;
        len++;
        str_char(&buffer, regex.str[i]);
      }
      if (flag == false) {
        printf("Not closing group at index: %d\n%s\n", old_i, regex.str);
        for (i = 0; i < old_i; i++) {
          printf(" ");
        }
        printf("^\n");
        return -1;
      }
      i--;
      r_op = group;
      break;
    case '{':
      len++;
      flag = false;
      old_i = i;
      str_char(&buffer, regex.str[i]);
      for (i = i + 1; i < regex.len && flag == false; i++) {
        if (regex.str[i] == '}')
          flag = true;
        len++;
        str_char(&buffer, regex.str[i]);
      }
      if (flag == false) {
        printf("Not closing range at index: %d\n%s\n", old_i, regex.str);
        for (i = 0; i < old_i; i++) {
          printf(" ");
        }
        printf("^\n");
        return -1;
      }
      i--;
      r_op = range;
      break;
    default:
      len++;
      str_char(&buffer, regex.str[i]);
      r_op = charr;
      // printf("default\n");
      // return Err;
      break;
    }
    (*size)++;
    (*list) = realloc((*list), (*size) * sizeof(regex_e));
    str_cpy(&(*list)[(*size) - 1].str, buffer);
    (*list)[(*size) - 1].r_op = r_op;
  }
  return Ok;
}

String string_regex_op(regex_op r_op) {
  String str_r_op;
  switch (r_op) {
  case 1:
    str_r_op = str_init("star", 5);
    break;
  case 2:
    str_r_op = str_init("question", 10);
    break;
  case 3:
    str_r_op = str_init("plus", 5);
    break;
  case 4:
    str_r_op = str_init("dot", 5);
    break;
  case 5:
    str_r_op = str_init("start_str", 15);
    break;
  case 6:
    str_r_op = str_init("end_str", 15);
  case 7:
    str_r_op = str_init("set", 15);
    break;
  case 8:
    str_r_op = str_init("group", 15);
    break;
  case 9:
    str_r_op = str_init("alter", 15);
    break;
  case 10:
    str_r_op = str_init("range", 15);
    break;
  case 11:
    str_r_op = str_init("charr", 15);
    break;
  case 12:
    str_r_op = str_init("escape", 15);
    break;
  }
  return str_r_op;
}
