#include <stdio.h>
#include <sys/stat.h>   // Per stat()
#include <string.h>     // Per strcmp()

// Includi la versione portabile di dirent.h per Windows
#include "libs/dirent.h"

#define MAX_PATH_LEN 1024

// #define SYMB1 '└'└─├│
// #define SYMB2 '─'
// #define SYMB3 '├'
// #define SYMB4 '│'

void list_files_recursively(const char *base_path, int depth) {
    DIR *dir;
    struct dirent *entry;
    struct stat statbuf;
    char path[MAX_PATH_LEN];

    if ((dir = opendir(base_path)) == NULL) {
        perror("opendir");
        return;
    }

    while ((entry = readdir(dir)) != NULL) {
        // Salta "." e ".."
        // if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
        if (entry->d_name[0] == '.')
            continue;

        // Costruisce il percorso completo (usa / o \ a seconda del sistema)
        snprintf(path, sizeof(path), "%s/%s", base_path, entry->d_name);

        if (stat(path, &statbuf) == -1) {
            perror("stat");
            continue;
        }

        // Stampa indentazione
        for (int i = 0; i < depth; i++) {
            printf("    ");
        }

        if (S_ISDIR(statbuf.st_mode)) {
            // È una directory
            printf("%s/\n", entry->d_name);
            list_files_recursively(path, depth + 1);  // Ricorsione
        } else {
            // È un file
            printf("%s\n", entry->d_name);
        }
    }

    closedir(dir);
}

int main() {
        printf("%c\n", (unsigned char)195);
    list_files_recursively(".", 0);  // Parte dalla directory corrente
    return 0;
}
