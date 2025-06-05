/*
 *  Zdrojový soubor hry Šibenice se vstupní funkcí main()
 *
 *  Zajišťuje:
 *  - přepnutí pracovního adresáře do složky programu
 *  - zpracování argumentů příkazové řádky
 *  - měření doby běhu programu
 *  - nastavení velikosti okna emulátoru terminálu
 *  - obsluhu hlavního menu
 *  - akce vykonávané před ukončením programu
 *    (uvolnění dynamické paměti, uložení dat atd.)
 *
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#if defined(_MSC_VER) || defined(__WATCOMC__)
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
#if TERM_COLOR == 1
  int term_color_zap = 1;
#else
  int term_color_zap = 0;
#endif

#ifndef OS_DOS
  int nastaveni_tabskore = 0;  /* když 1, skóre se vykresluje do tabulky */
#else
  int nastaveni_tabskore = 1;  /* když 1, skóre se vykresluje do tabulky */
#endif


struct tm *p_tmcas = NULL;


/*********************/


/* hlavičky lokálních funkcí */

/* zpracuje argumenty předané
   z příkazové řádky */
static void zpracuj_argumenty(int argc, char *argv_orig[]);

/* přepne do adresáře se spustitelným souborem
   kvůli korekci relativních cest */
static void prepni_adresar(int argc, char *argv[]);

#if TERM_SET == 1
  /* rozmery okna emulatoru terminalu [x_sloupce, y_radky] */
  static int term_rozmery[2] = { TERM_SIRKA, TERM_VYSKA };
#endif


/* vstupní bod aplikace */

int main(int argc, char *argv[])
{
  /* čas začátku běhu programu */
  time_t cas_spusteni = time(NULL);
  VOLBY_MENU volba_menu = MENU_MENU;

  /* přepnutí do složky s programem */
  prepni_adresar(argc, argv);

  /* zpracování argumentů CLI */
  zpracuj_argumenty(argc, argv);

  #ifdef OS_WEB
    /* vypnutí bufferování pro okamžitý výstup (např. Emscripten) */
    setvbuf(stdout, NULL, _IONBF, 0);  /* stdout */
    setvbuf(stderr, NULL, _IONBF, 0);  /* stderr - pro jistotu */
  #endif

  #if TERM_SET == 1
    /* inicializace ConHost na Windows */
    term_init();
  #endif

  /* nastavení terminálu (může změnit přepínač při spuštění) */
  #if TERM_SET == 1
    if (term_set == 1) {
      term_title(TERM_TITLE);
      term_size(term_rozmery[0], term_rozmery[1]);
      term_font(TERM_FONT, TERM_FONT_SZ, TERM_FONT_B);
    }
  #endif

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
  if (term_color_zap) {
    term_barvy_reset();
  }
  vymaz_obr();

  return 0;
}


/* definice lokálních funkcí */


static void zpracuj_argumenty(int argc, char *argv_orig[])
{
  int i, j, buffer_pozice = 0;
  char buffer_argv[65] = "";  /* pro zkopírování všech argumentů */
  char *argv[10];  /* pro ukazatele na začátky řetězců */

  /* převod na malá písmena a kopírování */

  if (argc > 1) {
    for (i = 0; i < argc; i++) {

      if (i >= arrlen(argv)) {  /* statické pole ukazatelů ! */
        fprintf(stderr, "%s(): Prilis mnoho argumentu...\n", __func__);
        exit(1);
      }

      argv[i] = buffer_argv + buffer_pozice;
      for (j = 0; argv_orig[i][j] != '\0'; j++, buffer_pozice++) {
        if (buffer_pozice + 1 > arrlen(buffer_argv)) {
          /* při testech zjištěno, že je potřeba buffer od 2 bajty (znaky) větší
             než je délka nejdelšího argumentu (vč. +1 přo '\0') - neopraveno */
          fprintf(stderr, "%s(): Nedostatecny buffer pro argumenty...\n", __func__);
          exit(1);
        }
        buffer_argv[buffer_pozice] = ((isupper(argv_orig[i][j]))
                                       ? tolower(argv_orig[i][j])
                                       : argv_orig[i][j]);
      }
      buffer_argv[buffer_pozice++] = '\0';
    }

    /* DEBUG */
      /*
      for (i = 0; i < argc; i++) {
        fprintf(stderr, "%d: \"%s\"\n", i, argv[i]);
      }
      */
    /* // DEBUG */
  }  /* // if (argc > 1) */

  /* implementace přepínačů */

  /* vypíše seznam dostupných přepínačů */
  if (argc == 2 && (strcmp(ARG_SIGN_1 ARG_HLP_SIGN_1, argv[1]) == 0
                    || strcmp(ARG_SIGN_2 ARG_HLP_SIGN_2, argv[1]) == 0
                    || strcmp(ARG_SIGN_3 ARG_HLP_SIGN_1, argv[1]) == 0
                    || strcmp(ARG_SIGN_3 ARG_HLP_SIGN_2, argv[1]) == 0)) {

    arg_hlp_text();

    #if (defined(OS_WIN) && TERM_SET == 1)  /* čekání před zavřením okna */
      puts("\n");
      fputs(PROMPT_ENTER_KONEC, stdout);
      cekej_enter();
    #endif
    exit(0);
  }
  /* zobrazení hlavní nápovědy */
  if (argc == 2 && (strcmp(ARG_SIGN_1 ARG_MAN_SIGN_1, argv[1]) == 0
                    || strcmp(ARG_SIGN_2 ARG_MAN_SIGN_2, argv[1]) == 0
                    || strcmp(ARG_SIGN_3 ARG_MAN_SIGN_1, argv[1]) == 0
                    || strcmp(ARG_SIGN_3 ARG_MAN_SIGN_2, argv[1]) == 0)) {

    #if TERM_SET == 1
      if (term_set == 1) {
        term_size(term_rozmery[0], term_rozmery[1]);
      }
    #endif

    napoveda();
    vymaz_obr();
    hlavicka_vykresli("Napoveda", TUI_HLAVICKA_TXT_P);
    putchar('\n');
    puts(ARG_HLP_TEXT "\n");
    #if (defined(OS_WIN) && TERM_SET == 1)  /* čekání před zavřením okna */
      puts("");
      fputs(PROMPT_ENTER_KONEC, stdout);
      cekej_enter();
    #endif
    exit(0);
  }
  /* zobrazení herních statistik */
  if (argc == 2 && (strcmp(ARG_SIGN_1 ARG_STA_SIGN_1, argv[1]) == 0
                    || strcmp(ARG_SIGN_2 ARG_STA_SIGN_2, argv[1]) == 0
                    || strcmp(ARG_SIGN_3 ARG_STA_SIGN_1, argv[1]) == 0
                    || strcmp(ARG_SIGN_3 ARG_STA_SIGN_2, argv[1]) == 0)) {

    #if TERM_SET == 1
      if (term_set == 1) {
        term_size(term_rozmery[0], term_rozmery[1]);
      }
    #endif

    if (stats_nastav()) {
      /* zjištění času poslední změny statistik */
      char s_cas[100] = "";
      time_t cas = time(NULL);
      p_tmcas = localtime(&cas);

      /*vymaz_obr();*/  /* nepraktické při přesměrování výstupu do soubor */
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

    #if (defined(OS_WIN) && TERM_SET == 1)  /* čekání před zavřením okna */
      puts("");
      fputs(PROMPT_ENTER_KONEC, stdout);
      cekej_enter();
    #endif
    exit(0);
  }
  /* vypíše informace o sestavení */
  if (argc == 2 && (strcmp(ARG_SIGN_1 ARG_VER_SIGN_1, argv[1]) == 0
                    || strcmp(ARG_SIGN_2 ARG_VER_SIGN_2, argv[1]) == 0
                    || strcmp(ARG_SIGN_3 ARG_VER_SIGN_1, argv[1]) == 0
                    || strcmp(ARG_SIGN_3 ARG_VER_SIGN_2, argv[1]) == 0)) {

    printf(ARG_VER_TEXT);

    #if (defined(OS_WIN) && TERM_SET == 1)  /* čekání před zavřením okna */
      puts("\n");
      fputs(PROMPT_ENTER_KONEC, stdout);
      cekej_enter();
    #endif
    exit(0);
  }

  /* vynutí spuštění v režimu malé herní obrazovky (80x25 znaků) */
  if      (argc == 2 && (strcmp(ARG_SIGN_1 ARG_DOS_SIGN_1, argv[1]) == 0
                         || strcmp(ARG_SIGN_2 ARG_DOS_SIGN_2, argv[1]) == 0
                         || strcmp(ARG_SIGN_3 ARG_DOS_SIGN_1, argv[1]) == 0
                         || strcmp(ARG_SIGN_3 ARG_DOS_SIGN_2, argv[1]) == 0)) {

    nastaveni_tabskore = 1;   /* zobrazi mensi herni obrazovku pro 80x25 zn. */
    #if TERM_SET == 1
      if (term_set == 1) {
        term_rozmery[0]  = TERM_SIRKA_DOS;  /* rozmery okna jako DOS shell */
        term_rozmery[1]  = TERM_VYSKA_DOS;
      }
    #endif
  }
  /* vypne term_set: automatické nastavení velikosti okna, písma, barev */
  else if (argc == 2 && (strcmp(ARG_SIGN_1 ARG_TER_SIGN_1, argv[1]) == 0
                         || strcmp(ARG_SIGN_2 ARG_TER_SIGN_2, argv[1]) == 0
                         || strcmp(ARG_SIGN_3 ARG_TER_SIGN_1, argv[1]) == 0
                         || strcmp(ARG_SIGN_3 ARG_TER_SIGN_2, argv[1]) == 0)) {

    #if TERM_SET == 1
      term_set = 0;
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

    #if TERM_SET == 1

      term_set = 1;

      for (i = 0; i < 2; i++) {  /* načtení X y Y */
        /* argumenty jsou 4: "PATH -w X Y" */
        int argument = i + 2;
        if (sscanf(argv[argument], "%d", term_rozmery + i) != 1) {
          goto chybne_argumenty;
        }
        else if (term_rozmery[i] <= 0) {  /* přípustnost hodnot */
          goto chybne_argumenty;
        }
      }

    #endif
  }
  /* chybné argumenty */
  else if (argc > 1) {
    chybne_argumenty:
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

  #if DEBUG == 1
    printf("--- %s(): ---\n", __func__);
    printf("Adresar 1: %s\n", cesta);
  #endif

  /* sestavení adresy dle specifik použitého OS */
  #if !defined(OS_WIN) && !defined(OS_DOS) && !defined(OS_MAC)
    strcat(cesta, lomitko);
    strcat(cesta, (argc > 0) ? argv[0] : "");
  #else
    strcpy(cesta, (argc > 0) ? argv[0] : "");
  #endif

  /* oříznutí konce adresy o název spustitelného souboru */
  p_char = strrchr(cesta, lomitko[0]);
  if (p_char != NULL)  *p_char = '\0';

  /* přepnutí adresáře */
  #if DEBUG == 1
    printf(
      "Adresar 2: %s\n"
      "-------------------------\n", cesta
    );
  #endif

  chdir(cesta);
}
