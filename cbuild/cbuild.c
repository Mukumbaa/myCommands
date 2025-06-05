#include "cbuild.h"

int main(int argc, char **argv){

  // Select your CC (supported only gcc for now)
  setCC("gcc");

  // Add your files
  addFile("main.c");
  addFile("add.c");

  // Add your falgs
  addFlag("-Wall -Wextra -Werror");
  
  // Add your libs
  // addLib("-Llib -lmylib");

  // To print the recap of your choises
  recap();

  setOutputName("prova");
  
  // To build
  build();

  // If your program need parameters from comand line before execution
  // param();

  // To run
  run();
  
  return 0;
}
