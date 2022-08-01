/*
 * hlavičkový soubor s globálními
 * konfiguračními konstantami
 */

#ifndef KONFIGURACE_H
#define KONFIGURACE_H

/* stringifikace maker */
#define xstr(a) str(a)
#define str(a) #a

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
  #define CCNAME   "Clang"
  #define CCVER    __clang_major__
  #define CCMINOR  __clang_minor__
#elif defined(__GNUC__)
  #if defined(__DJGPP__)
    #define CCNAME  "DJGPP"
  #else
    #define CCNAME  "GNU GCC"
  #endif
  #define CCVER    __GNUC__
  #define CCMINOR  __GNUC_MINOR__
#else
  #define CCNAME  "?"
  #define CCVER   (-1)
#endif
#ifndef CCMINOR
  #define CCMINOR  (-1)
#endif

/* GLOBÁLNÍ NASTAVENÍ */


/* úroveň ladění:
     0 - vypnuto
     1 - obrazovka není mazána
     2 - hádá se jedno přednastavené slovo */
#define DEBUG               0
#define DEBUG_HADANE_SLOVO  "Ah hoc Chrochrohcroch..."

#define NAZEV  "Hra Sibenice"
#define VERZE  "0.0.3"
#define AUTOR  "Martin TABOR (alias Ma-TA)"
#define WEB    "github.com/ma-ta"

/* volitelné externí soubory */
#define UKAZATELSIBENICE_SOUBOR  "./data/img_sibe.dat"
#define NAPOVEDA_SOUBOR          "./data/help_cze.dat"
/* nezbytné externí soubory */
#define HRA_SLOVA_SOUBOR         "./data/dict_cze.dat"
#define ERR_SOUBOR     "Nelze nacist externi soubor \"%s\"..."  /* informace o chybějícím souboru */

#define ANSI_FORMAT   1   /* zapne formátování ESC sekvencemi */
#ifdef OS_DOS
  #undef  ANSI_FORMAT
  #define ANSI_FORMAT  0
#endif
#define ZVUKY         1   /* zapne vkládání znaku '\a' */

#define POCET_KOL     9   /* celkový počet kol hry (hádaných slov) */
#define POCET_ZIVOTU  11  /* počet životů v jednom kole */

#define ERR_SIGN      "(!) "  /* vypysuje se před chybovou zprávou */

/* argumenty příkazové řádky */

/* definice uvozovacích znaků */
#define ARG_SIGN_1  "-"
#define ARG_SIGN_2  "--"
#define ARG_SIGN_3  "/"
/* definice krátkých a dlouhých přepínačů */
#define ARG_HLP_SIGN_1  "?"       /* zobrazí seznam přepínačů */
#define ARG_HLP_SIGN_2  "help"
#define ARG_MAN_SIGN_1  "m"       /* zobrazí hlavní nápovědu hry */
#define ARG_MAN_SIGN_2  "manual"
#define ARG_STA_SIGN_1  "s"       /* zobrazí herní statistiky */
#define ARG_STA_SIGN_2  "stats"
#define ARG_VER_SIGN_1  "v"       /* zobarzí informace o sestavení */
#define ARG_VER_SIGN_2  "ver"
#define ARG_DOS_SIGN_1  "d"       /* vynutí spuštení v režimu DOS */
#define ARG_DOS_SIGN_2  "dos"

#define ARG_VER_TEXT  "%s\n"  \
                      "Verze:   %s  (%s/%s %s%s, %s)\n"  \
                      "Web:     %s\n"  \
                      "Napsal:  %s\n"  \
                      , NAZEV  \
                      , VERZE, OSNAME, CCNAME  \
                      , (CCVER == (-1)) ? "" : xstr(CCVER)  \
                      , (CCVER == (-1) || CCMINOR == (-1)) ? "" : "." xstr(CCMINOR)  \
                      , __DATE__  \
                      , WEB  \
                      , AUTOR
#define ARG_HLP_TEXT  "  -" ARG_HLP_SIGN_1 ", --" ARG_HLP_SIGN_2 "\tzobrazi (tento) seznam prepinacu\n"  \
                      "  -" ARG_MAN_SIGN_1 ", --" ARG_MAN_SIGN_2 "\tzobrazi hlavni napovedu hry\n"  \
                      "  -" ARG_STA_SIGN_1 ", --" ARG_STA_SIGN_2 "\tzobrazi herni statistiky (kroniku vitezu)\n"  \
                      "  -" ARG_VER_SIGN_1 ", --" ARG_VER_SIGN_2 "\tzobrazi informace o sestaveni\n"  \
                      "  -" ARG_DOS_SIGN_1 ", --" ARG_DOS_SIGN_2 "\tvynuti spusteni v rezimu DOS"
#define ERR_ARGUMENTY  "Chybne argumenty prikazove radky...\n" \
                       "Pro napovedu spustte s prepinacem \"-?\" (resp. \"/?\" pro Win/DOS)."

#define arg_hlp_text()  printf("Pouziti: %s [PREPINAC]\n" ARG_HLP_TEXT "\n", argv[0])


/**********************/


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

#define konec()  { vymaz_obr();  \
                   hlavicka_vykresli(TUI_HLAVICKA_TXT_KONEC);  \
                   printf("\n\n" ARG_VER_TEXT "\n\n");  \
                   \
                   hra_vycisti();  \
                   ukazatelsibenice_vycisti(); }


/* tui - hlavička */

#define TUI_HLAVICKA_TXT         "v" VERZE  /* text v pravé dolní buňce */
#define TUI_HLAVICKA_TXT_KONEC   "Na shledanou"  /* (sudý počet zn. pro zarovnání na střed) */
#define TUI_HLAVICKA_BUNKA       26         /* šířka pravé spodní buňky */
#define ERR_TUI_HLAVICKA_ZAROV   "Text v hlavicce nelze zarovnat na stred (+/- zn.)..."
#define ERR_TUI_HLAVICKA_TXTMAX  "Text v hlavicce je delsi nez bunka... (max. " xstr(TUI_HLAVICKA_BUNKA) " zn.)"
#define TUI_HLAVICKA_OBR     \
"+-----------------------------------------------------+\n"  \
"|                                                     |\n"  \
"|         Ma-TA {}       H R A                        |\n"  \
"|            games            S I _ E _ I C E         |\n"  \
"|                                                     |\n"  \
"+--------------------------+--------------------------+\n"  \
"|    (c) 2022  Freeware    |%s|\n"                          \
"+--------------------------+--------------------------+\n"  \

/* hlavní menu */

#define MENU_SIRKA              55
#define ERR_MENU_NADPIS_TXTMAX  "Nadpis je delsi nez bunka..."
#define ERR_MENU_NADPIS_ZAROV   "Nadpis nelze zarovnat na stred (+/- zn.)..."
#define MENU_NADPIS             "M E N U"
#define MENU_NADPIS_L           ">>>>------>"
#define MENU_NADPIS_P           "<------<<<<"
#define MENU_ENUM               MENU_MENU, MENU_HRA, MENU_STATS, MENU_NAPOVEDA, MENU_KONEC
#define MENU_POLOZKY            "Nova hra", "Sibenicni kronika", "Napoveda", "Konec"
#define MENU_ZADEJ              "Zadejte cislo polozky"

 /* (strlen(DOBA_INFO) == MENU_SIRKA == šířka hlavičky == 55 zn.) */
#define DOBA_INFO  MENU_NADPIS_L "  Doba hrani:  %02d h %02d min %02d s  " MENU_NADPIS_P "\n\n\n"  \
                   , p_tmcas->tm_hour, p_tmcas->tm_min, p_tmcas->tm_sec

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

#define UKAZATELPISMEN_NIC  '\0'  /* v poli dostupných znaků nahradí použitou položku */
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
/* hlášky do pravého okénka (lichý počet znaků pro správné zarovnání!) */
#define UKAZATELSLOV_HLASKA      "* Vitej *"
#define UKAZATELSLOV_HLASKY_ANO  "VYBORNE", "BRAVO", "HEJ RUP", "TREFA", "ZASAH"
#define UKAZATELSLOV_HLASKY_NE   "TUDY NE", "NE NE", "AU!", "TO BOLELO", "VEDLE"

/* ukazatel šibenice */

#define UKAZATELSIBENICE_ZARAZKA  '#'

/* hra */

#define HRA_POCETSLOV       1000  /* počet slov načítaných ze slovníku */
#define HRA_POMOC_ZN_CENA   3     /* počet bodů za pomoc - doplnění písmene */
#define HRA_HLASKA_FORMAT   ansi_format(ANSI_INVER) ansi_format(ANSI_BLICK)
#define HRA_PROPOKRACOVANI  "(stiskni Enter...)"

/* VOLBA_KONEC    - ukončí probíhající hru
   VOLBA_VOLBY    - zobrazí dostupné volby
   VOLBA_NAPOVEDA - zobrazí velkou nápovědu
   VOLBA_POMOC_ZN - doplní písmeno za cenu určitého počtu bodů */
#define HRA_VOLBY           VOLBA_VOLBY    = '*',  \
                            VOLBA_KONEC    = '0',  \
                            VOLBA_NAPOVEDA = '1',  \
                            VOLBA_POMOC_ZN = '2'
/* seznam voleb */
#define HRA_VOLBY_SZN       "Ukoncit", "Manual" /*, "Napovez znak (za " xstr(HRA_POMOC_ZN_CENA) " b.)" */
/* počet položek musí být shodný (ne menší!) s HRA_VOLBY_SZN */
#define HRA_VOLBY_HODNOTY   VOLBA_KONEC, VOLBA_NAPOVEDA, VOLBA_POMOC_ZN
#define HRA_VOLBY_ZAVLP     "[]"  /* musí být 2 znaky! (strlen(HRA_VOLBY_ZN) >= 2, [0]=levý [1]=pravý */
#define HRA_VOLBY_SEP       "  "  /* oddělovač jednotlivých položek ve výpisu */


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
