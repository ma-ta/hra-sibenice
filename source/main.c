/*
 *  Zdrojový soubor hry Šibenice se vstupní funkcí main()
 *
 *  Zajišťuje:
 *  - zpracování argumentů příkazové řádky
 *  - přepnutí pracovního adresáře do složky programu
 *  - měření doby běhu programu
 *  - obsluhu hlavního menu
 *  - akce vykonávané před ukončením programu
 *    (uvolnění dynamické paměti, uložení dat atd.)
 *
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#ifdef _MSC_VER
  #include <direct.h>
#else
  #include <unistd.h>
#endif
#include "./globconf.h"  /* globální nastavení */
#include "./libs/ansi_fmt.h"  /* různé escape sekvence pro terminál */
#include "./menu/men_glob.h"  /* hlavní herní menu */
#include "./help/napoveda.h"  /* herní nápověda */
#include "./game/hra.h"  /* průběh hry */
#include "./game/game_tui/game_tui.h"  /* vykreslování elementů TUI */
#include "./tui/hlavicka.h"  /* vykreslování hlavičky s volitelnými texty */
#include "./stats/stats.h"  /* modul s herními statistikami */
#include "./libs/term_set.h"  /* nastavení titulku a rozměrů terminálu */


/* GLOBÁLNÍ PROMĚNNÉ */


#if TERM_SET == 1    /* logika nastavení v globconf.h */
  int term_set = 1;  /* zapne nastavení rozměrů a titulku terminálu */
#else
  int term_set = 0;
#endif

#ifndef OS_DOS
  int nastaveni_tabskore = 0;  /* když 1, skóre se vykresluje do tabulky */
#else
  int nastaveni_tabskore = 1;
#endif


struct tm *p_tmcas = NULL;


/*********************/


/* hlavičky lokálních funkcí */

/* zpracuje argumenty předané
   z příkazové řádky */
static void zpracuj_argumenty(int argc, char *argv[]);
/* přepne do adresáře se spustitelným souborem
   kvůli korekci relativních cest */
static void prepni_adresar(int argc, char *argv[]);


/* vstupní bod aplikace */

int main(int argc, char *argv[])
{
  /* čas začátku běhu programu */
  time_t cas_spusteni = time(NULL);
  VOLBY_MENU volba_menu = MENU_MENU;
  // pro nastavení velikosti terminálu (může změnit přepínač CLI)
  int term_rozmery[2] = { TERM_SIRKA, TERM_VYSKA };  /* [cols, rows] */

  /* přepnutí do složky s programem */
  prepni_adresar(argc, argv);

  /* zpracování argumentů CLI */
  zpracuj_argumenty(argc, argv);

  /* nastavení emulátoru terminálu */
  if (term_set == 1) {
    #ifdef OS_WIN
      /* zajistí otevření v okně starého Windows Console Host při současném
      použití přepínačů /link /subsystem:windows /entry:mainCRTStartup
      při kompilaci přes cl.exe */

      AllocConsole(); /* otevřen nové okno konzole (ConHost.exe) */
      /* přesměrování vstupů a výstupů do standardních I/O pro ConHost
        (program byl totiž spuštěn jako Windows GUI aplikace) */
      freopen("CONOUT$", "w", stdout);
      freopen("CONOUT$", "w", stderr);
      freopen("CONIN$", "r", stdin);
      /* přenesení okna terminálu do popředí (jinak je je nutné na okno
        terminálu po spuštění aplikace kliknout)
        vyžaduje připojení knihovny /link user32.lib */
      HWND hwnd = GetConsoleWindow();
      if (hwnd) {
        SetForegroundWindow(hwnd);
      }
    #endif

    term_title(TERM_TITLE);
    term_size(term_rozmery[0], term_rozmery[1]);
  }

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
        vymaz_obr();
        stats_vypis(false);
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
  fputs(ansi_format(ANSI_INVER)
        , stdout);
  printf(DOBA_INFO);
  fputs(ansi_format(ANSI_RESET)
        , stdout);
  puts("\n\n");
  fputs(PROMPT_ENTER_KONEC, stdout);
  cekej_enter();
  /*vymaz_obr();*/

  return 0;
}


/* definice lokálních funkcí */


static void zpracuj_argumenty(int argc, char *argv[])
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
    hlavicka_vykresli("Napoveda", TUI_HLAVICKA_TXT_P);
    putchar('\n');
    puts(ARG_HLP_TEXT "\n");
    exit(0);
  }
  /* zobrazení herních statistik */
  if (argc == 2 && (strcmp(ARG_SIGN_1 ARG_STA_SIGN_1, argv[1]) == 0
                    || strcmp(ARG_SIGN_2 ARG_STA_SIGN_2, argv[1]) == 0
                    || strcmp(ARG_SIGN_3 ARG_STA_SIGN_1, argv[1]) == 0
                    || strcmp(ARG_SIGN_3 ARG_STA_SIGN_2, argv[1]) == 0)) {
    if (stats_nastav()) {
      /* zjištění času poslední změny statistik */
      char s_cas[100] = "";
      time_t cas = time(NULL);
      p_tmcas = localtime(&cas);

      vymaz_obr();
      sprintf(s_cas, "%02d.%02d.%d %02d:%02d"
                     , p_tmcas->tm_mday, p_tmcas->tm_mon + 1, p_tmcas->tm_year + 1900
                     , p_tmcas->tm_hour, p_tmcas->tm_min);
      hlavicka_vykresli("Statistiky", s_cas);
      putchar('\n');
      stats_vypis(true);
      stats_vycisti();
      putchar('\n');
    }
    else  puts(STATS_ZADNE_STATS "\n");

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
  if      (argc == 2 && (strcmp(ARG_SIGN_1 ARG_DOS_SIGN_1, argv[1]) == 0
                         || strcmp(ARG_SIGN_2 ARG_DOS_SIGN_2, argv[1]) == 0
                         || strcmp(ARG_SIGN_3 ARG_DOS_SIGN_1, argv[1]) == 0
                         || strcmp(ARG_SIGN_3 ARG_DOS_SIGN_2, argv[1]) == 0)) {

    nastaveni_tabskore = 1;
  }
  /* vypne automatické nastavení velikosti okna terminálu */
  else if (argc == 2 && (strcmp(ARG_SIGN_1 ARG_TER_SIGN_1, argv[1]) == 0
                         || strcmp(ARG_SIGN_2 ARG_TER_SIGN_2, argv[1]) == 0
                         || strcmp(ARG_SIGN_3 ARG_TER_SIGN_1, argv[1]) == 0
                         || strcmp(ARG_SIGN_3 ARG_TER_SIGN_2, argv[1]) == 0)) {

    #if TERM_SET == 1
      term_set = 0;

      // DEBUG
      puts("term_set: vypnuto\n");
      cekej_enter();
      // DEBUG
    #endif
  }
  /* nastavi konkretni velikost okna terminalu */
  else if (argc == 4 && (strcmp(ARG_SIGN_1 ARG_TER_SIGN_1, argv[1]) == 0
                         || strcmp(ARG_SIGN_2 ARG_TER_SIGN_2, argv[1]) == 0
                         || strcmp(ARG_SIGN_3 ARG_TER_SIGN_1, argv[1]) == 0
                         || strcmp(ARG_SIGN_3 ARG_TER_SIGN_2, argv[1]) == 0)) {

    /* byl zadán přepínač pro změnu chování term_set a zbývají dva argumenty,
       zjistíme tedy, zda se jedná o parsovatelné a správné hodnoty
       pro nastavení rozměrů okna */

    /* TODO:
       PARSOVÁNÍ HODNOT Z ARGUMENTŮ A NASTAVENÍ term_rozmery[0..1] */

    #if TERM_SET == 1
      term_set = 1;

      // DEBUG
      puts("term_set: nastaveny rozmery X a Y\n");
      cekej_enter();
      // DEBUG
    #endif
  }
  /* chybné argumenty */
  else if (argc > 1) {
    fputs(ERR_SIGN ERR_ARGUMENTY "\n", stderr);
    exit(1);
  }
}


static void prepni_adresar(int argc, char *argv[])
{
  char lomitko[2]  = "";    /* podoba lomítka v závislosti na OS */
  char cesta[1000] = "";    /* buffer pro uložení path */
  char *p_char     = NULL;  /* pomocný ukazatel */

  /* nastavení podoby lomítka dle OS */
  #ifdef __DJGPP__
    lomitko[0] = '/';
  #elif defined(OS_DOS) || defined(OS_WIN)
    lomitko[0] = '\\';
  #else
    lomitko[0] = '/';
  #endif

  /* zjištění aktivní složky */
  (void) getcwd(cesta, sizeof(cesta));

  /* sestavení adresy dle specifik použitého OS */
  #if !defined(OS_WIN) && !defined(OS_DOS)
    strcat(cesta, lomitko);
    strcat(cesta, (argc > 0) ? argv[0] : "");
  #else
    strcpy(cesta, (argc > 0) ? argv[0] : "");
  #endif

  /* oříznutí konce adresy o název spustitelného souboru */
  p_char = strrchr(cesta, lomitko[0]);
  if (p_char != NULL)  *p_char = '\0';

  /* přepnutí adresáře */
  chdir(cesta);
}
