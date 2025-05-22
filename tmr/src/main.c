#include "../include/curses.h"
#include <stdio.h>
#include <time.h>


int main() {
    initscr();            // inizializza lo schermo
    noecho();             // non mostrare input utente
    curs_set(FALSE);      // nascondi cursore

    int y = 0;
    for (int i = 0; i < 10; i++) {
        mvprintw(y++, 0, "Riga numero %d", i);
        refresh();
        napms(500);       // attesa di mezzo secondo
    }

    getch();              // aspetta input prima di uscire
    endwin();             // ripristina terminale
    return 0;
}
