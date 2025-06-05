#include "../libs/ansi_fmt.h"
#include "../globconf.h"
#include "term_set.h"

#ifdef OS_WIN
  #include <windows.h>
#elif defined(OS_DOS)
   #include <dos.h>
#endif


// TEST MODULU - aktivace fce. main()
// (definováno také v globconf.h)
//===========================
#undef  DEBUG
#define DEBUG  0  // VYP => 0
//===========================

#if DEBUG == 1
  // AKTIVACE MODULU (jinak fce. nic nedělají)
  // (definováno také v globconf.h)
  //==========================================
  #undef  TERM_SET
  #define TERM_SET  1  // VYP => 0
  //==========================================

  // (DEBUG == 1)
  // ROZMĚRY OKNA JAKO ARGUMENTY PŘI SPUŠTĚNÍ
  //==========================================
  #define ARGS_ZAP  1  // VYP => 0
  //==========================================

  // NASTAVENÍ VLASTNÍCH ROZMĚRŮ
  // (definováno také globconf.h)
  //=============================
  //#undef   TERM_SIRKA
  //#undef   TERM_VYSKA
  //#define  TERM_SIRKA  73
  //#define  TERM_VYSKA  37
  //=============================
#endif


/* dodělávka pro přepínač [-w], který by měl vypnout i barvy */
extern int term_set;

static int ret_value  = EXIT_FAILURE;  /* návratová hodnota fcí. modulu */
static int pom_ret_val = 0;  /* pomocná proměnná */
static char system_prikaz[1000] = "";  /* textový buffer */

static term_color_bgfg term_bgfg = {
  .bg = TERM_BLACK,
  .fg = TERM_WHITE
};


void term_barvy(term_color pozadi, term_color text)
{
  #if TERM_SET == 1

    if (term_set) {  /* nouzová dodělávka pro přepínač [-w] */

        /* barvy jsou již nastaveny */
        if (pozadi == term_bgfg.bg && text == term_bgfg.fg) {
          return;
        }
        else {
          term_bgfg.bg = pozadi;
          term_bgfg.fg = text;
        }


        #if defined(OS_DOS)

          snprintf(
            system_prikaz
            , sizeof(system_prikaz)
            , "color %c%c"
            , (term_bgfg.bg <= 9) ? (term_bgfg.bg + '0') : (term_bgfg.bg + 'A' - 10)
            , (term_bgfg.fg <= 9) ? (term_bgfg.fg + '0') : (term_bgfg.fg + 'A' - 10)
          );

          system(system_prikaz);

        #elif defined(OS_WIN)  /* v ConHost funguje i řešení s [color] pro DOS výše */

          WORD win_text   = term_bgfg.fg;  /* dolní 4 bity, tj. 0-15 */
          WORD win_pozadi = ((term_bgfg.bg) << 4);  /* horní 4 bity */
          HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

          SetConsoleTextAttribute(hConsole, win_text | win_pozadi);

        #else  /* jiný systém než DOS a WIN => ANSI esc */

          #define LBARVY  7  /* posun v enum term_color */

          int ansi_text   = 30 + ((term_bgfg.fg <= LBARVY)
                                  ? (term_bgfg.fg)
                                  : (term_bgfg.fg - LBARVY));
          int ansi_pozadi = 40 + ((term_bgfg.bg <= LBARVY)
                                  ? (term_bgfg.bg)
                                  : (term_bgfg.bg - LBARVY));

          snprintf(
            system_prikaz
            , sizeof(system_prikaz)
            , CSI "%s%d" SGR CSI "%d" SGR
            , ((term_bgfg.fg > LBARVY) ? (ANSI_LIGHT) : (""))
            , ansi_text
            , ansi_pozadi
          );

          printf("%s", system_prikaz);
          fflush(stdout);

          /* DEBUG */
          fprintf(stderr, "%s\n", system_prikaz);
          /* // DEBUG */

     #endif  /* rozvětvení OS */

    }  /* // if (term_set) */

  #endif
}  /* term_barvy() */

void term_barvy_reset(void)
{
  #if TERM_SET == 1

    if (term_set) {  /* nouzová dodělávka pro přepínač [-w] */

      #if (defined(OS_DOS) || defined(OS_WIN))
        system("color");
      #else
        printf(CSI ANSI_RESET SGR);
        fflush(stdout);
      #endif

    }  /* // if (term_set) */

  #endif
}

void term_font(const char *font, int velikost, bool tucne)
{
  #if TERM_SET == 1

    /* kontrola argumentů */
    if (!font || velikost <= 0) {
      fprintf(stderr, ERR_SIGN "%s(): Chyba v argumentech...\n", __func__);
      return;
    }

    #ifdef OS_WIN

      wchar_t novy_font[LF_FACESIZE];  /* max velikost pro jméno fontu */
      MultiByteToWideChar(CP_UTF8, 0, font, -1, novy_font, LF_FACESIZE);

      HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

      CONSOLE_FONT_INFOEX cfi;
      cfi.cbSize = sizeof(cfi);
      cfi.nFont = 0;
      cfi.dwFontSize.X = 0;         /* šířka písma (0 => auto) */
      cfi.dwFontSize.Y = velikost;  /* výška písma */
      cfi.FontFamily = FF_MODERN;
      cfi.FontWeight = (tucne) ? FW_BOLD : FW_NORMAL;
      wcscpy(cfi.FaceName, novy_font);  /* název písma */

      if (!SetCurrentConsoleFontEx(hConsole, FALSE, &cfi)) {
          fprintf(stderr, ERR_SIGN "Nepodarilo se nastavit font...\n");
      }

    #endif

  #endif
}

bool term_title(const char *novy_titulek)
{
  #if DEBUG == 1
    printf(
      "--- ( %s(): DEBUG == 1 ) ---\n",
      __func__
    );
  #endif


  /* 1 => modul aktivován, 0 => deaktivován (fce. je možné stále volat) */
  #if TERM_SET == 1

    #ifdef OS_UNIX
      /* KDE Konsole - bohužel specifické chování
         (kód 30, nový titulek až po stisku klávesy...) */
      if (getenv("KONSOLE_VERSION")) {
        printf(ansi_osc_title_kde, novy_titulek);
      }
      /* obecný UN*X terminál
         testováno v:
         - GNOME 48 Terminal/Console
         - KDE Konsole 24.12.3
         - macOS 15.4 Terminal */
      else {
        printf(ansi_osc_title, novy_titulek);
      }
      ret_value = EXIT_SUCCESS;
    #elif defined(OS_WIN)
      // moderní Windows Terminal (fungují ESC, WinAPI nikoli)
      if (getenv("WT_SESSION")) {
        printf(ansi_osc_title, novy_titulek);
        ret_value = EXIT_SUCCESS;
      }
      // Windows Console Host (via WinAPI)
      else {
        SetConsoleTitle(novy_titulek)
          ? (ret_value = EXIT_SUCCESS)
          : (ret_value = EXIT_FAILURE);
      }
    #elif defined(OS_DOS)
      /* funkce nic nedělá */
      ret_value = EXIT_SUCCESS;
    #endif  // rozvětvení systémů

  #endif  /* TERM_SET == 1 */

  return ret_value;
}

/* prozatím funguje:
   - Windows Windows Console Host (ConHost.exe)
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

      /* nejjednodušší způsob pro Windows Console Host (ConHost.exe)
         v moderním Windows Terminal (WT) však pouze mění velikost
         bufferu bez změny velikosti okna (text se např. zalamuje)

         ve WT nemá smysl (bohužel, občas není proměnná WT_SESSION ani
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
      /* řešení pro macOS via AppleScript */

      pom_ret_val = snprintf(
        system_prikaz,
        sizeof(system_prikaz),

        "osascript "

        "-e 'tell application \"Terminal\" "
        "to set number of columns of front window to %d' "

        "-e 'tell application \"Terminal\" "
        "to set number of rows of front window to %d'",

        x, y
      );

      // pom_ret_val() je OK když snprintf() je kladná a menší než sizeof(n)
      if (pom_ret_val > 0 && pom_ret_val < (int) sizeof(system_prikaz)) {
        ret_value = (system(system_prikaz)
                      ? EXIT_FAILURE
                      : EXIT_SUCCESS);
      }
      else {
        #if DEBUG == 1
          fprintf(stderr, "(!) %s(): sizeof(system_prikaz)\n", __func__);
        #endif  // DEBUG
      }
    /* #ifdef OS_MAC */

    #elif defined(OS_DOS)
      /* funkce nic nedělá */
      ret_value = EXIT_SUCCESS;

    #endif  /* rozvětvení systémů */

  #endif  // TERM_SET == 1

  return ret_value;
}

/* nutné při spouštění programu na Windows
   - otevře terminál a přesměruje I/O, pokud byla aplikace
     kompilována s [/subsystem:windows /entry:mainCRTStartup] */
void term_init(void)
{
   #if TERM_SET == 1
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
   #endif
}


/* TEST MODULU */


#if (DEBUG == 1) && (ARGS_ZAP == 1)
  #define ARG_COUNT  2
  #define ARG_INFO  "Argumenty: COLS_X LINES_Y"
#endif


#if DEBUG == 1
  int main(int argc, char *argv[])
  {
    int ret_value = EXIT_SUCCESS;
    char stiskni_enter[] = "(stiskni Enter...)";
    int cmd_size[2] = { 0, 0 };


    term_init();

    // parsování argumentů

    #if ARGS_ZAP == 1
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
    #else
      cmd_size[0] = TERM_SIRKA;
      cmd_size[1] = TERM_VYSKA;
    #endif

    // informace pro uživatele

    printf("Nastavuji titulek okna na: %s\n", TERM_TITLE);
    ret_value = term_title(TERM_TITLE);

    printf(
      "Nastavuji velikost okna na X=%d, Y=%d\n",
      cmd_size[0], cmd_size[1]
    );

    // nastavení nové velikosti terminálu a test velikosti orámováním okna

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

      for (int i = 0; i < (int) sizeof(stiskni_enter) - 1 + (2); i++) {
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
