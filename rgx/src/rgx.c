#include "../include/rgx.h"
#include <complex.h>
#include <stdio.h>

bool match_here(String input, int *s_idx, regex_e re) {
  // printf("HERE-%c-%s\n",c,re.str.str);

  if (re.r_op == dot) {
    return input.str[(*s_idx)] != '\0'; // accetta qualsiasi carattere tranne fine stringa
  }
  if (re.r_op == set){
    int except_flag = str_containsr(re.str, "^", 2, NO_CASE_SENSITIVE);
    int rangeset_flag = str_containsr(re.str, "-", 2, NO_CASE_SENSITIVE);
    // int flag = true;
    

    for(int i = except_flag == -1 ? 0 : 1; i < re.str.len; i++){
      char ccurr = re.str.str[i];
      char ctmp;
      if (i+1 < re.str.len){
        ctmp = re.str.str[i+1];
      }
      if(ctmp == '-'){
        i+=2;
        ctmp = re.str.str[i];
        if(except_flag == -1){
          // printf("exceflag == -1\n");
          if(input.str[(*s_idx)] >= ccurr &&input.str[(*s_idx)]<= ctmp){
            return true;
          }
        }else{
          if(!(input.str[(*s_idx)]<ccurr || input.str[(*s_idx)]>ctmp)){
            //if the except is not fullfill
            return false;
          }
        }
      }else{
        if(except_flag == -1){
          if(input.str[(*s_idx)] == re.str.str[i]){
            return true;
          }
        }else{
          if(input.str[(*s_idx)] == re.str.str[i]){
            return false;
          }
        }
      }
    }
    
    if(except_flag != -1){
      return true;
    }
    return false;
  }
  for (int i = 0; i < re.str.len; i++) {
    if (input.str[(*s_idx)] == re.str.str[i]) {
      return true;
    }
  }
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
      // case range:
      //   r_idx++;
      //   break;
      // case alter:
      //   act = one;
      //   r_idx++;
      //   break;
      default:
        act = one;
        break;
      }
    }
    int match = 0;
    if (act == one) {
      if ((*s_idx) >= input.len){

        return false;
      }
      if (match_here(input, s_idx,curr)) {
        (*s_idx)++;
      } else {
        return false;
      }
    } else if (act == zeroOrOne) {
      if ((*s_idx) < input.len && match_here(input, s_idx, curr)) {
        (*s_idx)++;
      }
      // altrimenti, va bene anche 0 match
    } else if (act == zeroOrMore) {

      bool has_next = r_idx < size;
      regex_e next;
      if (has_next) {

        next = regex[r_idx];
      }

      if (curr.r_op == dot) {
        if (has_next == false) {

          return true;
        }

        while ((*s_idx) < input.len && match_here(input, s_idx, curr)) {
          if (match_here(input, s_idx, next)) {
            break;
          }
          (*s_idx)++;
        }

      } else {
        while ((*s_idx) < input.len && match_here(input, s_idx, curr)) {
          if (has_next && match_here(input, s_idx, next)) {
            break;
          }
          (*s_idx)++;
        }
      }

    } else if (act == oneOrMore) {
      if ((*s_idx) >= input.len || !match_here(input, s_idx, curr)) {
        return false;
      }
      (*s_idx)++;
      while ((*s_idx) < input.len && match_here(input, s_idx,curr)) {
        (*s_idx)++;
      }
    }
  }
  // return s_idx == input.len;
  return true;
}
void print_match(int start, int s_idx, String input) {
  // printf("Match\n");
  printf("start:%d-s_idx:%d\n", start, s_idx);
  printf("Match: %s\n", input.str);
  printf("       ");
  if (s_idx == 0) {
    for (int i = 0; i < input.len; i++) {
      printf("~");
    }
    printf("\n");
  } else {
    for (int i = 0; i < start; i++)
      printf(" ");
    for (int i = 0; i < s_idx; i++)
      printf("~");
    printf("\n");
  }
}
bool match_regex_anywhere(regex_e *regex, int size, String input) {
  int start = 0;
  int count = 0;
  int s_idx = 0;
  bool flagStart = false;
  bool flagEnd = false;

  if (regex[0].r_op == start_str) {
    regex = &regex[1];
    size--;
    flagStart = true;
  }
  if (regex[size - 1].r_op == end_str) {
    size--;
    flagEnd = true;
  }
  if (flagStart || flagEnd) {
    if (match_regex(regex, size, input, &s_idx) /*&& s_idx > 0*/) {

      bool print = false;
      if (flagStart == true) {
        if (start == 0) {

          print = true;
        } else {
          print = false;
        }
      }
      if (flagEnd == true) {
        if (s_idx == input.len - 1) {

          print = true;
        } else {
          print = false;
        }
      }
      if (print) {
        print_match(start, s_idx, input);
        return true;
      } else {
        printf("No match\n");
        return false;
      }
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
      print_match(start, s_idx, input);
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
      i++;
      old_i = i;
      str_char(&buffer, regex.str[i]);
      for (i = i + 1; i < regex.len && flag == false; i++) {
        if (regex.str[i] == ']'){
          flag = true;
        }
        len++;
        if (flag == false){
          str_char(&buffer, regex.str[i]);
        }
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
      str_cat(&(*list)[(*size) - 1].str, buffer, -1);
      (*list)[(*size) - 1].r_op = r_op;
      
      break;
    default:
      len++;
      str_char(&buffer, regex.str[i]);
      r_op = charr;
      // printf("default\n");
      // return Err;
      break;
    }
    if (r_op != range){
      
      (*size)++;
      (*list) = realloc((*list), (*size) * sizeof(regex_e));
      str_cpy(&(*list)[(*size) - 1].str, buffer);
      (*list)[(*size) - 1].r_op = r_op;
    }
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
    break;
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
