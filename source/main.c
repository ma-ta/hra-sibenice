#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "./globconf.h"
#include "./libs/ansi_fmt.h"
#include "./menu/men_glob.h"
#include "./help/napoveda.h"
#include "./game/hra.h"
#include "./game/game_tui/game_tui.h"
#include "./tui/hlavicka.h"
#include "./stats/stats.h"


/* GLOBÁLNÍ PROMĚNNÉ */


#ifndef OS_DOS
  int nastaveni_tabskore = 0;  /* když 1, skóre se vykresluje do tabulky */
#else
  int nastaveni_tabskore = 1;
#endif

struct tm *p_tmcas = NULL;


/*********************/


/* hlavičky funkcí */

/* zpracuje argumenty předané
   z příkazové řádky */
void zpracuj_argumenty(int argc, char *argv[]);


/* vstupní bod aplikace */

int main(int argc, char *argv[])
{
  /* čas začátku běhu programu */
  time_t cas_spusteni = time(NULL);
  VOLBY_MENU volba_menu = MENU_MENU;

  /* zpracování argumentů CLI */
  zpracuj_argumenty(argc, argv);

  /* vymaže obrazovku */
  vymaz_obr();
  /* inicializace generátoru pseudonáhodných čísel */
  srand((unsigned) time(NULL));
  /* inicializace modulu statistik */
  stats_nastav();

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

      case MENU_STATS:
        stats_vypis(false);
        fputs(ansi_cursor_off(), stdout);
        fputs(ansi_format(ANSI_INVER) "\n(Enter pro navrat...)" ansi_format(ANSI_RESET),
              stdout);
        cekej_enter();
        fputs(ansi_cursor_on(), stdout);
        vymaz_obr();
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
  stats_uloz();
  konec();
  /* zobrazení doby běhu programu */
  p_tmcas = gmtime((cas_spusteni = time(NULL) - cas_spusteni, &cas_spusteni));
  fputs(ansi_format(ANSI_INVER) ansi_format(ANSI_BLICK)
        , stdout);
  printf(DOBA_INFO);
  fputs(ansi_format(ANSI_RESET)
        , stdout);
  puts("\n\n");

  return 0;
}


void zpracuj_argumenty(int argc, char *argv[])
{
  /* vypíše seznam dostupných přepínačů */
  if (argc == 2 && (strcmp(ARG_SIGN_1 ARG_HLP_SIGN_1, argv[1]) == 0
                    || strcmp(ARG_SIGN_2 ARG_HLP_SIGN_2, argv[1]) == 0
                    || strcmp(ARG_SIGN_3 ARG_HLP_SIGN_1, argv[1]) == 0
                    || strcmp(ARG_SIGN_3 ARG_HLP_SIGN_2, argv[1]) == 0)) {

    arg_hlp_text();
    exit(0);
  }
  /* zobrazení hlavní nápovědy */
  if (argc == 2 && (strcmp(ARG_SIGN_1 ARG_MAN_SIGN_1, argv[1]) == 0
                    || strcmp(ARG_SIGN_2 ARG_MAN_SIGN_2, argv[1]) == 0
                    || strcmp(ARG_SIGN_3 ARG_MAN_SIGN_1, argv[1]) == 0
                    || strcmp(ARG_SIGN_3 ARG_MAN_SIGN_2, argv[1]) == 0)) {

    napoveda();
    vymaz_obr();
    konec();
    puts(ARG_HLP_TEXT "\n\n");
    exit(0);
  }
  /* zobrazení herních statistik */
  if (argc == 2 && (strcmp(ARG_SIGN_1 ARG_STA_SIGN_1, argv[1]) == 0
                    || strcmp(ARG_SIGN_2 ARG_STA_SIGN_2, argv[1]) == 0
                    || strcmp(ARG_SIGN_3 ARG_STA_SIGN_1, argv[1]) == 0
                    || strcmp(ARG_SIGN_3 ARG_STA_SIGN_2, argv[1]) == 0)) {

    if (stats_nastav())  stats_vypis(false);
    else                 puts(STATS_ZADNE_STATS "\n");

    exit(0);
  }
  /* vypíše informace o sestavení */
  if (argc == 2 && (strcmp(ARG_SIGN_1 ARG_VER_SIGN_1, argv[1]) == 0
                    || strcmp(ARG_SIGN_2 ARG_VER_SIGN_2, argv[1]) == 0
                    || strcmp(ARG_SIGN_3 ARG_VER_SIGN_1, argv[1]) == 0
                    || strcmp(ARG_SIGN_3 ARG_VER_SIGN_2, argv[1]) == 0)) {

    printf(ARG_VER_TEXT);
    exit(0);
  }

  /* vynutí spuštění v režimu DOS */
  if (argc == 2 && (strcmp(ARG_SIGN_1 ARG_DOS_SIGN_1, argv[1]) == 0
                    || strcmp(ARG_SIGN_2 ARG_DOS_SIGN_2, argv[1]) == 0
                    || strcmp(ARG_SIGN_3 ARG_DOS_SIGN_1, argv[1]) == 0
                    || strcmp(ARG_SIGN_3 ARG_DOS_SIGN_2, argv[1]) == 0)) {

    nastaveni_tabskore = 1;
  }
  /* chybné argumenty */
  else if (argc > 1) {
    fputs(ERR_SIGN ERR_ARGUMENTY "\n", stderr);
    exit(1);
  }
}
