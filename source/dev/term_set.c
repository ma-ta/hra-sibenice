#include <stdio.h>
#include <stdlib.h>
#include "../globconf.h"
#include "../libs/ansi_fmt.h"
#ifdef OS_WIN
  #include <windows.h>
#endif


// DEBUG definovano v globconf.h
/*
#undef  DEBUG
#define DEBUG  1
*/
#if DEBUG == 1
  #define ARG_COUNT  2
  #define ARG_INFO  "Argumenty:  COLS_X LINES_Y"
#endif


bool term_title(void)
{
  #if DEBUG == 1
    printf(
      "--- ( %s(): DEBUG == 1 ) ---\n",
      __func__
    );
  #endif


  bool ret_value = EXIT_FAILURE;

  #if TERM_SET == 1  // funkcionalita aktivována

    #ifdef OS_UNIX
      /* KDE Konsole - bohužel specifické chování
         (kód 30, nový titulek až po stisku klávesy...) */
      if (getenv("KONSOLE_VERSION")) {
        printf(ansi_osc_title_kde(TERM_TITLE));
      }
      /* obecný UN*X terminál
         testováno v:
         - GNOME 48 Terminal/Console
         - KDE Konsole
         - macOS 15 Terminal */
      else {
        printf(ansi_osc_title(TERM_TITLE));
      }
      ret_value = EXIT_SUCCESS;
    #elif defined(OS_WIN)
      // moderní Windows Terminal (fungují ESC, WinAPI nikoli)
      if (getenv("WT_SESSION")) {
        printf(ansi_osc_title(TERM_TITLE));
        ret_value = EXIT_SUCCESS;
      }
      // Windows Console Host (via WinAPI)
      else {
        SetConsoleTitle(TERM_TITLE)
          ? (ret_value = EXIT_SUCCESS)
          : (ret_value = EXIT_FAILURE);
      }
    #endif  // OS_WIN

  #endif  // TERM_SET == 1

  return ret_value;
}

/* ROZPRACOVÁNO - prozatím funguje:
   - Windows (CMD.EXE) */
bool term_size(int x, int y)
{
  #if DEBUG == 1
    printf(
      "--- ( %s(): DEBUG == 1 ) ---\n",
      __func__
    );
  #endif


  bool ret_value = EXIT_FAILURE;

  #if TERM_SET == 1

    #ifdef OS_WIN
      // ve WT se nemá vykonat (bohužel, občas WT_SESSION není ve WT zavedena)
      if (!getenv("WT_SESSION")) {
        /* nejjednodušší způsob pro Windows Console Host (CMD)
          v moderním Windows Terminal (WT) však pouze mění velikost
          bufferu bez změny velikosti okna (text se např. zalamuje) */

        char system_prikaz[20];  // "mode X, Y"
        int snprintf_ret = snprintf(
                            system_prikaz,
                            sizeof(system_prikaz),
                            "mode %d, %d",
                            x, y
                          );

        // snprintf() je OK když ret_val je kladná a < sizeof(n)
        if (snprintf_ret > 0 && snprintf_ret < sizeof(system_prikaz)) {
          ret_value = (system(system_prikaz)
                        ? EXIT_FAILURE
                        : EXIT_SUCCESS);
        }
        else {
          #if DEBUG == 1
            fprintf(stderr, "(!) %s(): sizeof(system_prikaz)\n", __func__);
          #endif  // DEBUG
        }
    }
    #endif  // OS_WIN

  #endif  // TERM_SET == 1

  return ret_value;
}



/* TEST MODULU */

#if DEBUG == 1
  int main(int argc, char *argv[])
  {
    int ret_value = EXIT_SUCCESS;
    char stiskni_enter[] = "(stiskni Enter...)";

    int cmd_size[2] = { 0, 0 };
    // parsování argumentů
    if (argc != ARG_COUNT + 1) {  // počet argumentů
      fprintf(stderr, ARG_INFO "\n");
      return EXIT_FAILURE;
    }
    for (int i = 0; i < ARG_COUNT; i++) {  // načítání argumentů
      int argument = i + 1;
      if (sscanf(argv[argument], "%d", cmd_size + i) != 1) {
        fprintf(
          stderr,
          "Chyba pri nacitani %d. argumentu: \"%s\"!\n",
          i, argv[argument]
        );
        return EXIT_FAILURE;
      }
      else if (cmd_size[i] <= 0) {  // ověření povolených hodnot
        fprintf(stderr, "X i Y musi byt vetsi nez 0!\n");
        return EXIT_FAILURE;
      }
    }

    puts("Nastavuji titulek okna na: " TERM_TITLE);
    ret_value = term_title();

    printf(
      "Nastavuji velikost okna na X=%d, Y=%d\n",
      cmd_size[0], cmd_size[1]
    );

    // nastavení nové velikosti okna a test velikosti výpisem znaků
    printf(stiskni_enter);
    cekej_enter();

    if (term_size(cmd_size[0], cmd_size[1]) == EXIT_SUCCESS) {
      vymaz_obr();

      int oramovani_zn;
      for (int radek = 0; radek < cmd_size[1]; radek++) {
        for (int sloupec = 0; sloupec < cmd_size[0]; sloupec++) {
          if (   radek == 0               || sloupec == 0
              || radek == cmd_size[1] - 1 || sloupec == cmd_size[0] - 1)
          {
            oramovani_zn = '#';
          }
          else {
            oramovani_zn = ' ';
          }
          putchar(oramovani_zn);
        }
        if (radek < cmd_size[1] - 1)  putchar('\n');  // konec řádku
      }

      for (int i = 0; i < sizeof(stiskni_enter) - 1 + (2); i++) {
        putchar('\b');
      }
      fflush(stdout);
      printf(" %s ", stiskni_enter);  // (+2)
      fflush(stdout);
    }
    else {
      ret_value = EXIT_FAILURE;
      puts("Nepodarilo se nastavit velikost okna.");
      printf(stiskni_enter);
    }

    cekej_enter();

    return ret_value;
  }
#endif  // DEBUG == 1