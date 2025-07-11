#include "../include/rgx.h"

int getNumber(String str)
{
    int number = 0.0;
    int pos = 0;
    if(str.str[pos] == '-'){
      return -1;
    }
    while(str.str[pos] >= '0' && str.str[pos] <= '9')
    {
        number = number * 10 + str.str[pos] - '0';
        pos++;
    }
    return number;
}
bool match_here(String input, int s_idx, regex_e re) {
  // printf("HERE-%c-%s\n",c,re.str.str);

  if (re.r_op == dot) {
    return input.str[(s_idx)] !=
           '\0'; // accetta qualsiasi carattere tranne fine stringa
  }
  if (re.r_op == set) {

    bool except_flag = re.str.str[0] == '^';
    int start_r_idx = 0;

    if (except_flag){
      start_r_idx = 1;
    }
    
    int size = 1;
    String *split = malloc(size * sizeof(String));
    split[0] = str_init("",1);
    for (int i = start_r_idx; i < re.str.len; i++){
      if(re.str.str[i] == '\\'){
        i++;
        if(re.str.str[i] == 'w'){
          size++;
          split = realloc(split, size*sizeof(String));
          split[size - 1] = str_init("a-zA-Z0-9_", 15);
        }else if(re.str.str[i] == 'W'){
          size++;
          split = realloc(split, size*sizeof(String));
          split[size - 1] = str_init("^a-zA-Z0-9_", 15);
        }else if(re.str.str[i] == 'd'){
          size++;
          split = realloc(split, size*sizeof(String));
          split[size - 1] = str_init("0-9", 15);
        }else if(re.str.str[i] == 'D'){
          size++;
          split = realloc(split, size*sizeof(String));
          split[size - 1] = str_init("^0-9", 15);
        }else{
          str_char(&split[0], re.str.str[i]);
        }
      }else{
        str_char(&split[0], re.str.str[i]);
      }
    }
    int offset = 0;
    if(split[0].len == 0){
      offset = 1;
      // split = &split[1];
      size--;
    }
    // printf("SPLIT\n");
    // for(int i = 0; i < size; i++){
    //   printf("%s\n",split[i].str);
    // }
    // printf("FINESPLIT\n");
    bool validate_set = false;
    for (int i = 0; i < size + offset; i++){
      bool single_set;
      
      bool flag_inner_except = false;
      flag_inner_except = split[i].str[0] == '^' ? true : false;
      start_r_idx = flag_inner_except ? 1 : 0;

      if (except_flag == flag_inner_except){
        single_set = false;
      }else{
        single_set = true;
      }
      
      for (int j = start_r_idx; j < split[i].len; j++){
        char ccurr = split[i].str[j];
        char ctmp = ' ';
        if (j + 1 < split[i].len) {
          ctmp = split[i].str[j+1];
        }
        if (ctmp == '-' && j + 2 < split[i].len) {
          j += 2;
          ctmp = split[i].str[j];

          if (except_flag == flag_inner_except){
            if(input.str[s_idx] >= ccurr && input.str[s_idx] <= ctmp){
              single_set = single_set || true;
            }
          }else{
            if(input.str[s_idx] >= ccurr && input.str[s_idx] <= ctmp){
              single_set = single_set && false;
            }
          }
          
        }else{
          if (except_flag == flag_inner_except){
            if (input.str[s_idx] == ccurr){
              single_set = single_set || true;
            }
          }else{
            if (input.str[s_idx] == ccurr){
              single_set = single_set && false;
            }
          }
        } // if | else
      } // second for
      validate_set = validate_set || single_set;
    } // first for
    for (int i = 0; i < size + offset; i++) {
        str_destroy(&split[i]);
    }
    // printf("free\n");
    free(split);
    return validate_set;
  }
  

  for (int i = 0; i < re.str.len; i++) {
    if (input.str[(s_idx)] == re.str.str[i]) {
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

    // if (regex[r_idx].r_op == range){
    //  return false; 
    // }

    
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
      case range:
        act = inRange;
        r_idx++;
        break;
      // case alter:
      //   act = one;
      //   r_idx++;
      //   break;
      default:
        act = one;
        break;
      }
    }
    if (act == one) {
      if ((*s_idx) >= input.len) {
        return false;
      }
      if (match_here(input, (*s_idx), curr)) {
        (*s_idx)++;
      } else {
        return false;
      }
    } else if (act == zeroOrOne) {
      if ((*s_idx) < input.len && match_here(input, (*s_idx), curr)) {
        (*s_idx)++;
      }
      // altrimenti, va bene anche 0 match
    } else if (act == zeroOrMore || act == oneOrMore) {

      if(act == oneOrMore){
        if ((*s_idx) >= input.len || !match_here(input, (*s_idx), curr)) {
          return false;
        }
        (*s_idx)++;
      }

      int start_s_idx = (*s_idx);

      while ((*s_idx) < input.len && match_here(input, (*s_idx), curr)) {
        (*s_idx)++;
      }

      for(int i = (*s_idx); i >= start_s_idx; i--){
        int temp_s_idx = i;
        String slice = {.str = &input.str[i], .len = input.len - i};
        
        if (match_regex(&regex[r_idx], size - r_idx, slice, &temp_s_idx)){
          (*s_idx) = i + temp_s_idx;
          return true;
        }
      }
      return false;

    }else if(act == inRange){

      String range = str_init(&regex[r_idx - 1].str.str[1], regex[r_idx - 1].str.len - 2);
      String *split;
      int dim = str_split(&split, range, ',', 50);
      if(dim != 2){
        return false;
      }
      int low = getNumber(split[0]);
      int high = getNumber(split[1]);
      int i;

      int start_s_idx = (*s_idx);

      for (i = 0; i < high; i++){
        if(!match_here(input, (*s_idx), curr)){
          break;
        }
        (*s_idx)++;
      }
      if (i < low || i > high){
        return false;
      }
      // printf("%d %d %d\n",low, high, i);
      // printf("i:%d j:%d\n",(*s_idx), start_s_idx + low);
      for(int i = (*s_idx); i >= start_s_idx + low; i--){
        int temp_s_idx = i;
        String slice = {.str = &input.str[i], .len = input.len - i};
        
        // printf("for\n");
        if (match_regex(&regex[r_idx], size - r_idx, slice, &temp_s_idx)){
          // printf("for\n");
          (*s_idx) = i + temp_s_idx;
          return true;
        }
      }
      // printf("qui %c\n",input.str[(*s_idx)]);
      return false;
    }
  }
  // return s_idx == input.len;
  return true;
}
void print_match(int start, int s_idx, String input) {
  // printf("Match\n");
  // printf("start:%d-s_idx:%d\n", start, s_idx);

  // bool color = false;
  printf("Match: ");
  for (int i = 0; i < input.len; i++) {
    if (i == start) {
      // Inizia evidenziazione
      printf("\033[44;37m");
    }
    printf("%c",input.str[i]);
    if (i == s_idx + start - 1) {
      // Termina evidenziazione
      printf("\033[0m");
    }
  }
  printf("\033[0m");
  printf("\n");

  // printf("Match: %s\n", input.str);
  // printf("       ");
  // if (s_idx == 0) {
  //   for (int i = 0; i < input.len; i++) {
  //     printf("~");
  //   }
  //   printf("\n");
  // } else {
  //   for (int i = 0; i < start; i++)
  //     printf(" ");
  //   for (int i = 0; i < s_idx; i++)
  //     printf("~");
  //   printf("\n");
  // }
}
bool match_regex_anywhere(regex_e *regex, int size, String input) {
  int start = 0;
  // int count = 0;
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
        // printf("No match\n");
        return false;
      }
    } else {
      // printf("No match\n");
      return false;
    }
  }
  while (start < input.len) {
    s_idx = 0;
    String sliced = {.str = &input.str[start], .len = input.len - start};

    if (match_regex(regex, size, sliced, &s_idx) /*&& s_idx > 0*/) {
      //ADDED IF
      if(s_idx != 0){
        count++;
        print_match(start, s_idx, input);
      }
      if (s_idx == 0) {
        s_idx++;
      }
      // Avanza fino alla fine del match
      start += s_idx;
      }else{
        
      if (s_idx == 0) {
        s_idx++;
      }
      start += s_idx; 
      // printf("else\n");
    }
  }

  if (count != 0) {
    return true;
  }

  // printf("No match\n");
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
      break;
    case '\\':
      len++;
      if (i + 1 < regex.len){
        i++;
      }
      if (regex.str[i] == 'w'){
        str_catr(&buffer, "a-zA-Z0-9_",15);
        r_op = set;
      }
      else if (regex.str[i] == 'W'){
        str_catr(&buffer, "^a-zA-Z0-9_",15);
        r_op = set;
      }
      else if (regex.str[i] == 'd'){
        str_catr(&buffer, "0-9",15);
        r_op = set;
      }
      else if (regex.str[i] == 'D'){
        str_catr(&buffer, "^0-9",15);
        r_op = set;
      }
      else{
        str_char(&buffer, regex.str[i]);
        r_op = charr;
      }
      break;
    case '[':
      len++;
      flag = false;
      i++;
      old_i = i;
      // str_char(&buffer, regex.str[i]);
      for (i = i; i < regex.len && flag == false; i++) {
        // printf("set: %c\n",regex.str[i]);
        if (regex.str[i] == ']') {
          flag = true;
        }
        len++;
        if (flag == false) {

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
      // str_cat(&(*list)[(*size) - 1].str, buffer, -1);
      // (*list)[(*size) - 1].r_op = r_op;

      break;
    default:
      len++;
      str_char(&buffer, regex.str[i]);
      r_op = charr;
      // printf("default\n");
      // return Err;
      break;
    }
    // if (r_op != range) {

      (*size)++;
      (*list) = realloc((*list), (*size) * sizeof(regex_e));
      str_cpy(&(*list)[(*size) - 1].str, buffer);
      (*list)[(*size) - 1].r_op = r_op;
    // }
  }
  return Ok;
}

bool sanitize_regex(regex_e *list, int size){

  for(int i = 0; i < size; i++){
    switch (list[i].r_op) {
      case star:
      case plus:
      case question:
        if(i + 1 < size){
          if(list[i + 1].r_op == star || list[i + 1].r_op == plus ||list[i + 1].r_op == question){
            return false; 
          }
        }
      default:
        break;
    }
  }
  return true;
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

