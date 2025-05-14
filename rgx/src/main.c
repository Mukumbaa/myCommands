#include "../include/rgx.h" //libstr.h stdbool.h
#include <stdio.h>
#include <stdlib.h>


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
// bool match_regex_anywhere(regex_e *regex, int size, String input) {
//   int s_idx = 0;
//   int count = 0;
//   for (int start = 0; start < input.len; start++) {
//     // printf("start%d\n",start);
//     String sliced = {.str = &input.str[start], .len = input.len - start};
//     // printf("!%s-%d\n",sliced.str,sliced.len);
//     if (match_regex(regex, size, sliced, &s_idx)) {
//       count++;
//       printf("start:%d-s_idx:%d\n",start,s_idx);
//       printf("Match: %s\n", input.str);
//       printf("       ");
//       for (int i = 0; i < start; i++) {
//         printf(" ");
//       }
//       for (int i = 0; i < s_idx; i++) {
//         printf("~");
//       }
//       printf("\n");
//       // return true;
//     }
//   }
//   if (count != 0){
//     return true;
//   }
//   printf("No match\n");
//   return false;
// }
bool match_regex_anywhere(regex_e *regex, int size, String input) {
  int start = 0;
  int count = 0;

  while (start < input.len) {
    int s_idx = 0;
    String sliced = {.str = &input.str[start], .len = input.len - start};

    if (match_regex(regex, size, sliced, &s_idx) && s_idx > 0) {
      count++;
      printf("start:%d-s_idx:%d\n", start, s_idx);
      printf("Match: %s\n", input.str);
      printf("       ");
      for (int i = 0; i < start; i++)
        printf(" ");
      for (int i = 0; i < s_idx; i++)
        printf("~");
      printf("\n");

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

int main(int argc, char **argv) {

  String regex = str_init(argv[1], 100);
  String input = str_init(argv[2], 100);
  // String input = str_init("abba", 100);
  int ret;
  int size = 0;
  regex_e *list = NULL;
  list = malloc(size * sizeof(regex_e));

  // Parse regex
  ret = parse_regex(regex, &list, &size);
  if (ret == Err) {
    return -1;
  }
  for (int i = 0; i < size; i++) {
    printf("%s-%s\n", list[i].str.str, string_regex_op(list[i].r_op).str);
  }

  bool r = match_regex_anywhere(list, size, input);
  printf("%d\n", r);

  return 0;
}
