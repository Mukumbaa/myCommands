#include <math.h>
#include <stdio.h>
// #include <stdlib.h>

#if defined(_WIN32) || defined(_WIN64)
#include <io.h>
#include <fcntl.h>
#define isatty _isatty
#define fileno _fileno
#else
#include <unistd.h>
#endif

int main(void) {
    // Controlla se stdin è associato a un terminale
    if (!isatty(fileno(stdin))) {
        // Stampa il contenuto di stdin
        int c;
        while ((c = getchar()) != EOF) {
            putchar(c);
        }
    } else {
        fprintf(stderr, "Nessun input da stdin (nessuna pipe o redirezione).\n");
    }

    return 0;
}
