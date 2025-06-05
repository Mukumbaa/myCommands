#ifndef CB
#define CB

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>

static char* CC = NULL;
static char** FLAG = NULL;
static char** LIB = NULL;
static char **FILES = NULL;
static char PARAM[256] = "\0";

static int numFlags = 0;
static int numLibs = 0;
static int numFiles = 0;

static void setCC(char *cc){
  CC = cc;
}
static void printFiles(){
  printf("Files:\n");
  for(int i = 0; i < numFiles; i++){
    printf("\t%s\n", FILES[i]);
  }
}
static void printFlags(){
  printf("Flags:\n\t");
  for(int i = 0; i < numFlags; i++){
    printf("%s",FLAG[i]);
  }
  printf("\n");
}
static void printLibs(){
  printf("Libs:\n\t");
  for(int i = 0; i < numLibs; i++){
    printf("%s", LIB[i]);
  }
  printf("\n");
}
static void recap(){
  printf("CC: %s\n",CC);
  printFiles();
  printFlags();
  printLibs();
  printf("PARAM:\n\t%s\n",PARAM);
}
static void addFile(char *file_name){
  
  // Alloca spazio per un nuovo puntatore
  char **temp = (char **) realloc(FILES, (numFiles + 1) * sizeof(char *));
  if (temp == NULL) {
      perror("Errore realloc");
      exit(EXIT_FAILURE);
  }

  FILES = temp;

  // Copia il nome del file
  FILES[numFiles] = (char *) malloc(strlen(file_name) + 1);
  if (FILES[numFiles] == NULL) {
      perror("Errore malloc");
      exit(EXIT_FAILURE);
  }

  strcpy(FILES[numFiles], file_name);
  numFiles++;
}
static void addFlag(char *flag){
  // Alloca spazio per un nuovo puntatore
  char **temp = (char **) realloc(FLAG, (numFlags + 1) * sizeof(char *));
  if (temp == NULL) {
      perror("Errore realloc");
      exit(EXIT_FAILURE);
  }

  FLAG = temp;

  // Copia il nome del file
  FLAG[numFlags] = (char *) malloc(strlen(flag) + 1);
  if (FLAG[numFlags] == NULL) {
      perror("Errore malloc");
      exit(EXIT_FAILURE);
  }

  strcpy(FLAG[numFlags], flag);
  numFlags++;
  
}
static void addLib(char *lib){
  // Alloca spazio per un nuovo puntatore
  char **temp = (char **) realloc(LIB, (numLibs + 1) * sizeof(char *));
  if (temp == NULL) {
      perror("Errore realloc");
      exit(EXIT_FAILURE);
  }

  LIB = temp;

  // Copia il nome del file
  LIB[numLibs] = (char *) malloc(strlen(lib) + 1);
  if (LIB[numLibs] == NULL) {
      perror("Errore malloc");
      exit(EXIT_FAILURE);
  }

  strcpy(LIB[numLibs], lib);
  numLibs++;
  
}
static void build(){
    // 1. Crea cartella obj/ se non esiste
    struct stat st = {0};
    if (stat("obj", &st) == -1) {
        system("mkdir obj");
        // mkdir("obj");
    }

    // 2. Compila ogni file in obj/file.o
    for (int i = 0; i < numFiles; i++) {
        char command[512] = {0};
        char obj_path[256] = {0};

        // Estrai nome base del file (es. "main.c" → "main.o")
        const char* src = FILES[i];
        const char* filename = strrchr(src, '/');
        filename = filename ? filename + 1 : src;

        snprintf(obj_path, sizeof(obj_path), "obj/%.240s", filename);
        char* dot = strrchr(obj_path, '.');
        if (dot) strcpy(dot, ".o");

        // Costruisci comando
        snprintf(command, sizeof(command), "%s -c", CC ? CC : "gcc");

        for (int j = 0; j < numFlags; j++) {
            strncat(command, " ", sizeof(command) - strlen(command) - 1);
            strncat(command, FLAG[j], sizeof(command) - strlen(command) - 1);
        }

        strncat(command, " ", sizeof(command) - strlen(command) - 1);
        strncat(command, src, sizeof(command) - strlen(command) - 1);

        strncat(command, " -o ", sizeof(command) - strlen(command) - 1);
        strncat(command, obj_path, sizeof(command) - strlen(command) - 1);

        printf("[Compilazione] %s\n", command);
        system(command);
    }

    // 3. Linka tutti i .o in "output"
    char link_cmd[1024] = {0};
    snprintf(link_cmd, sizeof(link_cmd), "%s", CC ? CC : "gcc");

    for (int j = 0; j < numFlags; j++) {
        strncat(link_cmd, " ", sizeof(link_cmd) - strlen(link_cmd) - 1);
        strncat(link_cmd, FLAG[j], sizeof(link_cmd) - strlen(link_cmd) - 1);
    }

    for (int i = 0; i < numFiles; i++) {
        const char* src = FILES[i];
        const char* filename = strrchr(src, '/');
        filename = filename ? filename + 1 : src;

        char obj_path[256] = {0};
        snprintf(obj_path, sizeof(obj_path), "obj/%.240s", filename);
        char* dot = strrchr(obj_path, '.');
        if (dot) strcpy(dot, ".o");

        strncat(link_cmd, " ", sizeof(link_cmd) - strlen(link_cmd) - 1);
        strncat(link_cmd, obj_path, sizeof(link_cmd) - strlen(link_cmd) - 1);
    }

    for (int j = 0; j < numLibs; j++) {
        strncat(link_cmd, " ", sizeof(link_cmd) - strlen(link_cmd) - 1);
        strncat(link_cmd, LIB[j], sizeof(link_cmd) - strlen(link_cmd) - 1);
    }

    strncat(link_cmd, " -o output", sizeof(link_cmd) - strlen(link_cmd) - 1);

    printf("[Linking] %s\n", link_cmd);
    system(link_cmd);    
}
static void run(){
  printf("Run:\n");
  if (PARAM[0] != '\0'){
    char command[256];
    snprintf(command, sizeof(command),"output.exe %s", PARAM);
    system(command);
    printf("\nEnd run.");
    return;
  }
  system("output.exe");  
  printf("\nEnd run.");
}
static void param(){
  printf("Inser param:\n");
  fgets(PARAM, sizeof(PARAM),stdin);
  PARAM[strcspn(PARAM, "\n")] = 0;
  // printf("param: %s\n",PARAM);
}




#endif
