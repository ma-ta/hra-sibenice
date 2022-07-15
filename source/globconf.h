/*
 * hlavičkový soubor s globálními
 * konfiguračními konstantami
 */

#ifndef KONFIGURACE_H
#define KONFIGURACE_H


#include <stdbool.h>

/* detekce vývojového prostředí */
#if defined(__linux__) || defined(__APPLE__)
  #ifdef __APPLE__
    #define OSNAME  "macOS"
  #else
    #define OSNAME  "Linux"
  #endif
#elif defined(_WIN32) || defined(_WIN64)
  #define OSNAME  "Windows"
#elif defined(__MSDOS__)
  #define OS_DOS
  #define OSNAME  "DOS"
#else
  #define OSNAME  "?"
#endif

#if defined(_MSC_VER)
  #define CCNAME  "MSVC"
  #define CCVER   _MSC_VER
#elif defined(__WATCOMC__)
  #define CCNAME  "Watcom"
  #define CCVER   (-1)
#elif defined(__clang__)
  #define CCNAME  "Clang"
  #define CCVER   __clang_major__
#elif defined(__GNUC__)
  #if defined(__DJGPP__)
    #define CCNAME  "DJGPP"
  #else
    #define CCNAME  "GNU GCC"
  #endif
  #define CCVER   __GNUC__
#else
  #define CCNAME  "?"
  #define CCVER   (-1)
#endif


/* globální nastavení */

/* úroveň ladění:
     0 - vypnuto
     1 - obrazovka není mazána
     2 - hádá se jedno přednastavené slovo */
#define DEBUG               0
#define DEBUG_HADANE_SLOVO  "Ah hoc Chrochrohcroch..."

#define NAZEV         "Hra Sibenice"
#define VERZE         "0.0.2"
#define AUTOR         "Martin TABOR (Ma-TA)"
#define WEB           "github.com/ma-ta"

/* volitelné externí soubory */
#define UKAZATELSIBENICE_SOUBOR  "./data/sibenice.dat"
#define NAPOVEDA_SOUBOR          "./data/napoveda.dat"
/* nezbytné externí soubory */
#define HRA_SLOVA_SOUBOR         "./data/slovcz1k.dat"

#define ERR_SIGN      "(!) "  /* vypysuje se před chybovou zprávou */
#define ERR_SOUBOR    "Nelze nacist externi soubor \"%s\"..."  /* informace o chybějícím souboru */
#define ANSI_FORMAT   1       /* zapne formátování ESC sekvencemi */
#define ZVUKY         1       /* zapne vkládání znaku '\a' */

#define POCET_KOL     9   /* celkový počet kol hry (hádaných slov) */
#define POCET_ZIVOTU  11  /* počet životů v jednom kole */


/* detekce OS pro příkaz vymazání obrazovky */
#if (DEBUG == 0)
  #if (defined(_WIN32) || defined(_WIN64) || defined(__MSDOS__))
    #define vymaz_obr()  system("cls")
  #else
    #define vymaz_obr()  system("clear")
  #endif
#else
  #define vymaz_obr()
#endif


#define cekej_enter()  while (getchar() != '\n')  ;

#define konec()  { hra_vycisti(); ukazatelsibenice_vycisti(); }

/* tui - hlavička */

#define TUI_HLAVICKA_OBR  \
"+-----------------------------------------------------+\n"  \
"|                                                     |\n"  \
"|               H R A                                 |\n"  \
"|                       S I B E N I C E               |\n"  \
"|                                                     |\n"  \
"+--------------------------+--------------------------+\n"  \
"|      (c) 2022 Ma-TA      |        ver. %s        |\n"     \
"+--------------------------+--------------------------+\n"  \

/* hlavní menu */

#define MENU_SIRKA    55
#define MENU_ENUM     MENU_MENU, MENU_HRA, MENU_NAPOVEDA, MENU_KONEC
#define MENU_POLOZKY  "Nova hra", "Napoveda", "Konec"

/* ukazatele */

#define UKAZATELE_ORAMOVANI_ZN          '-'
#define UKAZATELE_ORAMOVANI_KRIZENI_ZN  '+'
#define UKAZATELE_SIRKA_BUNKY           5

/* ukazatel kol */

#define UKAZATELKOL_BARVA       ANSI_BLUE
#define UKAZATELKOL_BLIKAT      1
#define UKAZATELKOL_FORMAT      ansi_format(ANSI_BOLD)  /* ansi_frcolor(UKAZATEL_BARVA) */  ansi_format(ANSI_INVER)
#define UKAZATELKOL_SYM_VLEVO   ">"
#define UKAZATELKOL_SYM_VPRAVO  "<"

/* ukazatel slov */

#define UKAZATELSLOV_PROMPT      ">  "  /* znak před hádaným slovem */
#define UKAZATELSLOV_VELKA       0  /* vypysuje vše velkými písmeny */
#define UKAZATELSLOV_PISMENO_CH  0  /* určuje, zda brát CH jako jedno písmeno */

/* ukazatel písmen */

#define UKAZATELPISMEN_NIC  '0'
#define UKAZATELPISMEN_DOSTUPNA_PISMENA_SIZE  ('Z' - 'A' + 1)
#define ukazatelpismen_dostupna_pismena_napln(pole)  {  int znak;  \
                                                        for (znak = 'A'; znak < 'A' + UKAZATELPISMEN_DOSTUPNA_PISMENA_SIZE; znak++) {  \
                                                          *((pole) + (znak - 'A')) = (char) znak;  \
                                                        }  \
                                                     }

/* ukazatel slov */

#define UKAZATELSLOV_DELKA_MAX   31
#define UKAZATELSLOV_MASKA       '_'
#define UKAZATELSLOV_PRESKOCIT   " "  ","  "."  "?"  "!"  "-"  ";"  "%%"  "'"  "\""
#define UKAZATELSLOV_HLASKA_MAX  (UKAZATELE_SIRKA_BUNKY * 2)
#define UKAZATELSLOV_HLASKA      "Vitejte"
#define UKAZATELSLOV_HLASKY_ANO  "VYBORNE", "BRAVO", "HEJ RUP", "TREFA", "ZASAH"
#define UKAZATELSLOV_HLASKY_NE   "TUDY NE", "NE NE", "AU!", "TO BOLELO", "VEDLE"

/* ukazatel šibenice */

#define UKAZATELSIBENICE_ZARAZKA  '#'

/* hra */

#define HRA_POCETSLOV      1000
#define HRA_HLASKA_FORMAT  ansi_format(ANSI_INVER) ansi_format(ANSI_BLICK)
#define HRA_PROPOKRACOVANI  "(stiskni Enter...)"

#define HRA_OBR_PROHRA  \
"          |\n"  \
"       ---+---\n"  \
"          |\n"  \
"          |\n"  \
"        R I P\n"

#define HRA_OBR_VYHRA  \
"          ooooooooo\n"  \
"        oo   O o   oo\n"  \
"       ooo    |    ooo\n"  \
"       oo  ~~ O ~~  oo  ->  \" Uff... OK. \"\n"  \
"        ooooooooooooo\n"

#define HRA_HLASKA_VYHRA   "VYHRAL JSI S CELKOVYM SKORE "  \
                           ansi_format(ANSI_BOLD) "%d BODU" ansi_format(ANSI_RESET) HRA_HLASKA_FORMAT " !"
#define HRA_HLASKA_PROHRA  "TENTOKRAT TO NEVYSLO! Ziskavas 0 bodu."

/* nápověda */

#define NAPOVEDA_ZARAZKA    '#'
#define NAPOVEDA_OBRAZOVEK  4


#endif
