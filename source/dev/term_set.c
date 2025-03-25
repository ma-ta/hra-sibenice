/* TODO
 * - zkusit lepší detekci Windows Terminal např. dle rodičovského procesu,
 * - přepsat AppleSkript tak, aby přijímal argumenty fce. X a Y,
 * - umožnit nastavení titulku okna argumentem fce., ne symbol. konst.
 */


#include <stdio.h>
#include <stdlib.h>
#include "../globconf.h"
#include "../libs/ansi_fmt.h"
#ifdef OS_WIN
  #include <windows.h>
#endif


// DEBUG definovano take v globconf.h

#undef  DEBUG
#define DEBUG  1


#if DEBUG == 1
  #define ARG_COUNT  2
  #define ARG_INFO  "Argumenty:  COLS_X LINES_Y"
#endif


static int ret_value = EXIT_FAILURE;  // návratová hodnota fcí. modulu
static int pom_ret_val = 0;  // pomocná proměnná
static char system_prikaz[1000] = "";  // textový buffer


bool term_title(void)
{
  #if DEBUG == 1
    printf(
      "--- ( %s(): DEBUG == 1 ) ---\n",
      __func__
    );
  #endif



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
         - KDE Konsole 24.12.3
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
   - Windows CMD
   - macOS Terminal */
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

      #if DEBUG == 1
        puts("OS_WIN");
        cekej_enter();
      #endif

      /* nejjednodušší způsob pro Windows Console Host (CMD)
        v moderním Windows Terminal (WT) však pouze mění velikost
        bufferu bez změny velikosti okna (text se např. zalamuje) */

      /* ve WT nemá smysl (bohužel, občas není proměnná WT_SESSION ani
         ve WT zavedena - je tedy vhodné napsat jinou implementaci,
         která WT spolehlivě detekuje např. podle rodičovského procesu) */
      if (!getenv("WT_SESSION")) {

        int pom_ret_val = snprintf(
                            system_prikaz,
                            sizeof(system_prikaz),
                            "mode con: cols=%d lines=%d",
                            x, y
                          );

        // pom_ret_val() je OK když snprintf() je kladná a menší než sizeof(n)
        if (pom_ret_val > 0 && pom_ret_val < sizeof(system_prikaz)) {
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
    // #ifdef OS_WIN

    #elif defined(OS_MAC)
      /* řešení pro macOS via AppleScript - bohužel v pixelech,
        cílem je tedy jen odhadnout dostatečně velkou plochu,
        kód nenastavuje konkrétní počet řádků a znaků na řádek */

      pom_ret_val = snprintf(
        system_prikaz,
        sizeof(system_prikaz),

        "osascript -e '"
        "tell application \"Terminal\" "
        "to set bounds of front window to "
        "{%d, %d, %d, %d}'",

        300, 100, 1050, 850
      );

      // pom_ret_val() je OK když snprintf() je kladná a menší než sizeof(n)
      if (pom_ret_val > 0 && pom_ret_val < sizeof(system_prikaz)) {
        ret_value = (system(system_prikaz)
                      ? EXIT_FAILURE
                      : EXIT_SUCCESS);
      }
      else {
        #if DEBUG == 1
          fprintf(stderr, "(!) %s(): sizeof(system_prikaz)\n", __func__);
        #endif  // DEBUG
      }

    #endif  // OS_MAC

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
      printf("%s", stiskni_enter);
      cekej_enter();
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
    printf("%s", stiskni_enter);
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
      printf("%s", stiskni_enter);
    }

    cekej_enter();

    return ret_value;
  }
#endif  // DEBUG == 1
