#include <stdio.h>
#include <sys/stat.h>   // Per stat()
#include <string.h>     // Per strcmp()
#include "libs/dirent.h"

#define MAX_PATH_LEN 1024

int fileTreeRec(const char *base_path, int depth) {
    DIR *dir;
    struct dirent *entry;
    struct stat statbuf;
    char path[MAX_PATH_LEN];
    int numOfFiles = 0;

    if ((dir = opendir(base_path)) == NULL) {
        perror("opendir");
        return -1;
    }
    
    while ((entry = readdir(dir)) != NULL) {
        // skip "." end ".."
        // if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
        if (entry->d_name[0] == '.'){
            continue;
        }

        // Costruisce il percorso completo (usa / o \ a seconda del sistema)
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
            // È una directory
            printf("%s/\n", entry->d_name);
            int isEmpty = fileTreeRec(path, depth + 1) > 0 ? 0 : 1;
        } else {
            // È un file
            printf("%s\n", entry->d_name);
        }
        numOfFiles++;
    }
    closedir(dir);
    return numOfFiles;
}

int main() {
    fileTreeRec(".", 0);
    return 0;
}
