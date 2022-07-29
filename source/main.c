#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "./globconf.h"
#include "./menu/menu_main.h"
#include "./help/napoveda.h"
#include "./game/hra.h"
#include "./game/ukazatele/ukazatel_sibenice.h"
#include "./tui/hlavicka.h"


/* GLOBÁLNÍ PROMĚNNÉ */


#ifndef OS_DOS
  int nastaveni_tabskore = 0;  /* když 1, skóre se vykresluje do tabulky */
#else
  int nastaveni_tabskore = 1;
#endif


/*********************/


/* hlavičky funkcí */

/* zpracuje argumenty předané
   z příkazové řádky */
void zpracuj_argumenty(int argc, char *argv[]);


/* vstupní bod aplikace */

int main(int argc, char *argv[])
{
  /* zpracování argumentů CLI */
  zpracuj_argumenty(argc, argv);

  VOLBY_MENU volba_menu = MENU_MENU;

  /* vymaže obrazovku */
  vymaz_obr();

  /* inicializace generátoru pseudonáhodných čísel */
  srand((unsigned) time(NULL));

  /* hlavní menu */

  while (volba_menu != MENU_KONEC)
  {
    switch (volba_menu = menu())
    {
      case MENU_HRA:
        /* nastaví parametry hry */
        hra_nastav(POCET_KOL, POCET_ZIVOTU);
        /* spustí hru */
        hra_vysledek(hra_start());
        break;
      
      case MENU_NAPOVEDA:
        /* zobrazí nápovědu */
        napoveda();
        break;
      
      /* provede se, když je volba neplatná */
      default:
        vymaz_obr();
        break;
    }
  }

  /* akce před ukončením programu */
  vymaz_obr();
  hlavicka_vykresli(TUI_HLAVICKA_TXT_KONEC);
  printf("\n\n" ARG_VER_TEXT "\n\n");
  konec();

  return 0;
}


void zpracuj_argumenty(int argc, char *argv[])
{
  /* vypsání verze programu */
  if (argc == 2 && (strcmp(ARG_SIGN_1 "v", argv[1]) == 0
                    || strcmp(ARG_SIGN_2 "version", argv[1]) == 0
                    || strcmp(ARG_SIGN_3 "v", argv[1]) == 0
                    || strcmp(ARG_SIGN_3 "version", argv[1]) == 0)) {
    printf(ARG_VER_TEXT);
    exit(0);
  }
  /* vypsání nápovědy */
  if (argc == 2 && (strcmp(ARG_SIGN_1 "?", argv[1]) == 0
                    || strcmp(ARG_SIGN_2 "help", argv[1]) == 0
                    || strcmp(ARG_SIGN_3 "?", argv[1]) == 0
                    || strcmp(ARG_SIGN_3 "help", argv[1]) == 0)) {
    printf(ARG_HELP_TEXT);
    exit(0);
  }
  /* vynutí spuštění v režimu DOS */
  if (argc == 2 && (strcmp(ARG_SIGN_1 "d", argv[1]) == 0
                    || strcmp(ARG_SIGN_2 "dos", argv[1]) == 0
                    || strcmp(ARG_SIGN_3 "d", argv[1]) == 0
                    || strcmp(ARG_SIGN_3 "dos", argv[1]) == 0)) {
    nastaveni_tabskore = 1;
  }
  /* chybné argumenty */
  else if (argc > 1) {
    fputs(ERR_SIGN ERR_ARGUMENTY "\n", stderr);
    exit(1);
  }
}
