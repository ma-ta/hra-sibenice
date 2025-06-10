#ifdef POKUS  /* vyřazení souboru z kompilace */


/*
 *  (TESTOVACÍ SOUBOR - rozpracováno)
 *
 *  Nastavení velikosti okna emulátoru (GNOME Terminal, ...)
 *  v UN*X systémech -> k implementaci do term_set.c
 */


#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <unistd.h>

#define SIRKA  74
#define VYSKA  34

int main(void)
{
    int vychozi_x, vychozi_y;
    struct winsize w;

    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    vychozi_y = w.ws_row;
    vychozi_x = w.ws_col;

    system("clear");

    printf(
      "Okno ma vysku: %d radku a sirku %d sloupecku.\n"
      , vychozi_y
      , vychozi_x
    );

    printf(
      "Nastavuji velikost %d x %d zn.\n"
      "(stiskni Enter...)"
      , SIRKA
      , VYSKA
    );

    /* řídicí sekvence XTerm */
    printf("\033[8;%d;%dt", VYSKA, SIRKA);
    fflush(stdout);

    while (getchar() != '\n')
      ;  /* čekání na Enter */

    /* navrácení původní velikosti - při spuštění programu */
    printf("\033[8;%d;%dt", vychozi_y, vychozi_x);
    fflush(stdout);

    return 0;
}


#endif
