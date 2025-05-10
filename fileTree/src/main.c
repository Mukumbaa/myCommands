#include <stdio.h>
#include <sys/stat.h>   // Per stat()
#include <string.h>     // Per strcmp()
#include <stdlib.h>

#ifdef _WIN32
#include <windows.h>
#include "../include/dirent.h"
#define CLEAR_SCREEN() system("cls")
#define SLEEP_SECONDS(sec) Sleep((sec) * 1000)
#else
#include <dirent.h>
#include <unistd.h>
#define CLEAR_SCREEN() system("clear")
#define SLEEP_SECONDS(sec) sleep(sec)
#endif

#define MAX_PATH_LEN 1024

void fileTreeRec(const char *base_path, int depth) {
    DIR *dir;
    struct dirent *entry;
    struct stat statbuf;
    char path[MAX_PATH_LEN];

    if ((dir = opendir(base_path)) == NULL) {
        perror("opendir");
        return;
    }
    
    while ((entry = readdir(dir)) != NULL) {
        // skip "." end ".."
        // if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
        if (entry->d_name[0] == '.'){
            continue;
        }

        // build path
        snprintf(path, sizeof(path), "%s/%s", base_path, entry->d_name);

        if (stat(path, &statbuf) == -1) {
            perror("stat");
            continue;
        }
        // indentation
        for (int i = 0; i < depth; i++) {
            if(i < 1){
                printf("  ");
            }
            else{
                printf("|  ");
            }
        }
        if(!(depth < 1)){
            printf("--");
        }

        if (S_ISDIR(statbuf.st_mode)) {
            // is a dir
            printf("%s/\n", entry->d_name);
            fileTreeRec(path, depth + 1);
        } else {
            // is a file
            printf("%s\n", entry->d_name);
        }
    }
    closedir(dir);
    return;
}

int main(int argc, char **argv) {

    argc = argc - 1;
    int flagDynamic = 0;

    if(argc == 1){
        if(strcmp(argv[1],"-d") == 0){
            flagDynamic = 1;
        }
    }else if(argc > 1){
        printf("Too many arguments\nUsage: ftree [-d]\n");
        return 0;
    }
    // printf("%d\n", flagDynamic);

    if(flagDynamic){
        
        while (1) {
            CLEAR_SCREEN();
            fileTreeRec(".", 0);
            SLEEP_SECONDS(2);
        }
    }else{
        printf("\n");
        fileTreeRec(".", 0);
        printf("\n");
    }
    
    return 0;
}
