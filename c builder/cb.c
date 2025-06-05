#include "cb.h"

int main(int argc, char **argv){
  setCC("gcc");
  addFile("main.c");
  addFlag("-Wall -Wextra -Werror");
  recap();
  build();
  // param();
  run();
  return 0;
}
