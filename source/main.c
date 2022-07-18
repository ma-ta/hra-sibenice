#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "./globconf.h"
#include "./menu/menu_main.h"
#include "./help/napoveda.h"
#include "./game/hra.h"
#include "./game/ukazatele/ukazatel_sibenice.h"


/* GLOBÁLNÍ PROMĚNNÉ */


int nastaveni_tabskore = 0;  /* když 1, skóre se vykresluje do tabulky */
#ifdef OS_DOS
  nastaveni_tabskore = 1;
#endif


/*********************/


int main(int argc, char *argv[])
{
  /* zpracování argumentů CLI */
  if (argc > 1) {
    if (argc == 2 && (strcmp("-v", argv[1]) == 0 || strcmp("--version", argv[1]) == 0)) {
      printf("%s\n"
             "Verze:   %s  (%s/%s %d, %s)\n"
             "Web:     %s\n"
             "Napsal:  %s\n"
             , NAZEV
             , VERZE, OSNAME, CCNAME, CCVER, __DATE__
             , WEB
             , AUTOR
            );
      return 0;
    }
    else {
      fputs(ERR_SIGN "Chybne argumenty prikazove radky...\n", stderr);
      return 1;
    }
  }

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

  /* vymaže obrazovku před ukončením */
  vymaz_obr();

  konec();

  return 0;
}
