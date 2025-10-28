#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include "../globconf.h"
#include "./game_tui/game_tui.h"
#include "./hra.h"
#include "../libs/ansi_fmt.h"
/*
#include "../libs/krypto.h"
*/
#include "../tui/hlavicka.h"
#include "../help/napoveda.h"
#include "../stats/stats.h"


typedef enum {
  HRA_VOLBY
} ZVLASTNI_VOLBY;
static char *volby_seznam[] = { HRA_VOLBY_SZN };  /* popisky zvláštních voleb */
static int  volby_hodnoty[] = { HRA_VOLBY_HODNOTY };
static FILE *f_slova = NULL;  /* soubor se slovy k hádání */
static char **slova = NULL;  /* pole pro uložení slov ze souboru */
static int slova_size = 0; /* pro uložení zjištěného počtu slov ve slovníku */
bool slova_nactena = false;

extern int hlaska_fmt;            /* slouží provizorně k resetování formátu hlášky v modulu tui_slov.c */

static int hra_probiha  = false;
static int pocet_kol    = 0;
static int kolo_hry     = 0;
static int celkem_bodu  = 0;
static int pocet_zivotu = 0;
static int zbyva_zivotu = 0;


/* hlavičky lokálních funkcí */
static int hra_kolo(void);
static bool nacti_slova(void);
static bool slova_duplicita(char *slovo, char **seznam, int seznam_size);


/* veřejné funkce */

void hra_vysledek(int skore)
{
  int umisteni = 0;  /* pro uložení pozice z stats_zpracuj_body() */

  /* započítání herního času */
  stats_zpracuj_cas(true, (skore > 0) ? true : false);

  if (term_color_zap) {
    if (skore > 0) {
      term_barvy(TERM_GREEN, TERM_LWHITE);
    }
    else {
      term_barvy(TERM_BLACK, TERM_LRED);
    }

    vymaz_obr();
  }

  hlavicka_vykresli(TUI_HLAVICKA_TXT_L, TUI_HLAVICKA_TXT_P);
  puts("\n");

  fputs("   >   " HRA_HLASKA_FORMAT, stdout);
  /* výhra */
  if (skore > 0) {
    printf(HRA_HLASKA_VYHRA, skore);
    fputs(ansi_format(ANSI_RESET), stdout);
    /* hráč se umístil na jednom z TOP míst */
    if ((umisteni = stats_zpracuj_body(skore)) > 0) {
      fputs("\n       ", stdout);
      if (umisteni == 1)  puts("To je "
                               ansi_format(ANSI_BLICK)
                               ansi_format(ANSI_BOLD)
                               "nejlepsi"
                               ansi_format(ANSI_RESET)
                               " dosazeny vysledek !");
      else                printf("To je "
                                 ansi_format(ANSI_BOLD)
                                 "%d. nejlepsi"
                                 ansi_format(ANSI_RESET)
                                 " vysledek v poradi !\n", umisteni);
      puts("\n\n" HRA_OBR_VYHRA);
      fputs("\n\n       ", stdout);
      printf(ansi_format(ANSI_BOLD)
             "Uved sve jmeno do kroniky:  "
             ansi_format(ANSI_RESET));
      stats_zadej_jmeno(umisteni);
      vymaz_obr();
      stats_vypis(false);
      return;
    }
    /* dosažené skóre není významné */
    else {
      printf("\n       (Zatim nejvyssi dosazene skore je %d b...)\n", stats_zjisti_nte_nejbody(1));
      puts("\n\n" HRA_OBR_VYHRA);
    }
  }
  /* prohra */
  else {
    fputs(HRA_HLASKA_PROHRA, stdout);
    fputs(ansi_format(ANSI_RESET), stdout);
    puts("\n\n\n" HRA_OBR_PROHRA);
  }

  /* čekání na stisk klávesy Enter */
  fputs("\n\n   " HRA_PROPOKRACOVANI, stdout);
  cekej_enter();
}


void hra_vycisti(void)
{
  register int i = 0;

  if (slova) {
    for (i = 0; i < slova_size; i++) {
      if (slova[i]) {
        free((void *) slova[i]);
        slova[i] = NULL;
      }
    }
    free((void *) slova);
    slova = NULL;
    slova_size = 0;
  }
}

void hra_nastav(int kol, int zivotu) {
  pocet_kol    = kol;
  kolo_hry     = 1;
  celkem_bodu  = 0;
  pocet_zivotu = zivotu;
  zbyva_zivotu = zivotu;
  hlaska_fmt   = -1;      /* reset (odbarvení) formátu hlášky před novou hrou */

  if (!nacti_slova()) {
    hra_probiha = false;
    fputs(ERR_SIGN "Hru nelze spustit.\n", stderr);
    fputs(ansi_format(ANSI_INVER) "\n(Enter pro ukonceni...)" ansi_format(ANSI_RESET), stdout);
    cekej_enter();
    vymaz_obr();
    konec();
    exit(1);
  }

  ukazatelsibenice_nastav(zbyva_zivotu, celkem_bodu);
  ukazatelkol_nastav(kolo_hry, pocet_kol, UKAZATELE_SIRKA_BUNKY);
  #if (DEBUG == 2)
    ukazatelslov_nastav(DEBUG_HADANE_SLOVO);
  #else
    /* DEBUG - vypíše hádaná slova a jejich počet na stderr */
      /*
      fprintf(stderr, "slova_size: %d\n\n", slova_size);
      for (int i = 0; i < slova_size; i++) {
        if (slova[i]) {
          fprintf(stderr, "\"%s\"\n", slova[i]);
        }
        else  {
          fprintf(stderr, "NULL\n");
        }
      }
      */
    /* //DEBUG */

    ukazatelslov_nastav(slova[rand() % slova_size]);
  #endif
  ukazatelpismen_nastav(pocet_kol, UKAZATELE_SIRKA_BUNKY);
  ukazatelpismen_nastav(pocet_kol, UKAZATELE_SIRKA_BUNKY);

  hra_probiha = true;
}

int hra_start(void) {
  int bodu_kolo = 0;

  /* začátek měření herního času */
  stats_zpracuj_cas(false, false);

  ukazatelslov_hlaska(UKAZATELSLOV_HLASKA);
  while (hra_probiha && kolo_hry <= pocet_kol) {
    bodu_kolo = hra_kolo();

    if (bodu_kolo < 1) {
      /* slovo jsi neuhodl */
      celkem_bodu = 0;

      if (term_color_zap) {
        term_barvy(TERM_BLACK, TERM_LRED);
      }
      ukazatelslov_odkryj();
      vymaz_obr();
      ukazatele_vykresli();
      #if (ZVUKY == 1)
        putchar('\a');
      #endif
      fputs(ansi_format(ANSI_INVER) HRA_HLASKA_NEUHODL ansi_format(ANSI_RESET) "  " HRA_PROPOKRACOVANI, stdout);
      cekej_enter();
      vymaz_obr();
      break;
    }
    else {
      /* další tah */
      celkem_bodu += bodu_kolo;
      zbyva_zivotu = pocet_zivotu;
      kolo_hry++;
      ukazatelslov_hlaska("");
      ukazatelsibenice_nastav(pocet_zivotu, celkem_bodu);
      ukazatelkol_nastav(kolo_hry, pocet_kol, UKAZATELE_SIRKA_BUNKY);
      #if (DEBUG == 2)
        ukazatelslov_nastav(DEBUG_HADANE_SLOVO);
      #else
        ukazatelslov_nastav(slova[rand() % slova_size]);
      #endif
      ukazatelpismen_nastav(pocet_kol, UKAZATELE_SIRKA_BUNKY);
    }
  }

  if (celkem_bodu > 0) {
    vymaz_obr();
  }

  return celkem_bodu;
}


/* lokální funkce */


static bool nacti_slova(void)
{
  register int i = 0;
  char *p_char = NULL;
  char **pp_char = NULL;
  int c = '\0';  /* pomocná proměnná pro načítání znaků */
  char slovo[100] = ""; /* pomocný buffer pro načítání slov ze souboru */
  int pocet_slov = 0;  /* pro zjištění počtu slov v souboru */
  int pocet_slov_min = (HRA_POCETSLOV_MIN > 0) ? HRA_POCETSLOV_MIN : 1;
  bool nacteno = true;  /* nastavuje na konci fce., zda byla slova načtena */
  bool ret_val = true;  /* návratová hodnota fce. */

  /* ukončí funkci, pakliže jsou slova již načtena */
  if (slova_nactena) {
    return true;
  }
  else {
    printf("      |\n      +--> "
      ansi_format(ANSI_INVER)
      ansi_format(ANSI_BLICK)
        "Nacitam slova ze slovniku..."
      ansi_format(ANSI_RESET)
    );
    fflush(stdout);
  }

  /* otevře soubor se slovy */
  if ((f_slova = fopen(HRA_SLOVA_SOUBOR, "r")) == NULL) {
    vymaz_obr();
    fprintf(stderr, "\n" ERR_SIGN ERR_SOUBOR "\n", HRA_SLOVA_SOUBOR);
    goto f_nacti_slova_konec;
  }

  /* zjištění počtu slov v souboru */

  while (fgets(slovo, sizeof(slovo), f_slova)) {
    /* odstranění znaků konce řádku */

    /* odstranění \n z bufferu + kontrola načtení celého řádku */
    if ((p_char = strchr(slovo, '\n')) != NULL) {
      *p_char = '\0';
    }
    else {
      /* případ, kdy se do bufferu nevešel celý řádek */
      while ((c = fgetc(f_slova)) != '\n' && c != EOF)
        ;  /* přeskočení zbytku řádku */
    }
    if ((p_char = strchr(slovo, '\r')) != NULL)  *p_char = '\0';

    /* přeskočení řádku ve specifických případech */

    /* přeskočení - prázdný řádek */
    if (strlen(slovo) < 1) {
      fputs(ERR_SIGN "Preskocen prazdny radek ve slovniku...\n", stderr);
      continue;
    }
    /* přeskočení - obsahuje mezeru ' ' */
    else if (strchr(slovo, ' ') != NULL) {
      fputs(ERR_SIGN "Radek slovniku nesmi obsahovat mezery (' ')...\n", stderr);
      continue;
    }
    /* zvýšení čítače slov v souboru */
    else {
      pocet_slov++;
    }
  }

  /* příliš malý počet slov v souboru */
  if (pocet_slov < pocet_slov_min) {
    vymaz_obr();
    fprintf(
      stderr
      , ERR_SIGN "Min. pocet slov ve slovniku je %d,\n"
        "    dostupnych aktualne max. %d slov...\n"
      , HRA_POCETSLOV_MIN
      , pocet_slov
    );
    nacteno = false;
    ret_val = false;
    goto f_nacti_slova_konec;
  }

  /* alokace pole pro slova (pole ukazatelů na char*) */
  if (!(slova = (char **) malloc(pocet_slov * sizeof(char *)))) {
    vymaz_obr();
    fprintf(stderr, ERR_SIGN "Nedostatek volne pameti pro %d slov...\n", pocet_slov);
    nacteno = false;
    ret_val = false;
    goto f_nacti_slova_konec;
  }

  /* uložení velikosti pole do glob. proměnné */
  slova_size = pocet_slov;

  /* inicializace hodnot v alokovaném poli */
  for (i = 0; i < slova_size; i++) {
    slova[i] = NULL;
  }

  /* načte slova do paměti */

  /* návrat na začátek souboru */
  fseek(f_slova, 0L, SEEK_SET);

  i = 0;  /* hlídá rozsah pole a postupně vkládá slova */
  while (fgets(slovo, sizeof(slovo), f_slova) && i < slova_size)
  {
    /* odstranění znaků konce řádku */

    /* odstranění \n z bufferu + kontrola načtení celého řádku */
    if ((p_char = strchr(slovo, '\n')) != NULL) {
      *p_char = '\0';
    }
    else {
      /* případ, kdy se do bufferu nevešel celý řádek */
      fprintf(
        stderr
        , ERR_SIGN "Nacitany radek je delsi nez buffer[%d]...\n"
        , (int) sizeof(slovo)
      );

      pocet_slov--;

      while ((c = fgetc(f_slova)) != '\n' && c != EOF)
        ;  /* přeskočení zbytku řádku */

      continue;
    }
    /* odstranění z bufferu \r */
    if ((p_char = strchr(slovo, '\r')) != NULL)  *p_char = '\0';

    /* přeskočení řádku ve specifických případech */

    /* přeskočení - prázdný řádek (pocet_slov -> viz výše) */
    if (strlen(slovo) < 1) {
      continue;
    }
    /* přeskočení - obsahuje mezeru ' ' (pocet_slov -> viz výše) */
    else if (strchr(slovo, ' ') != NULL) {
      continue;
    }

    /* dešifrování slova
       upraveno: slovník bude načten do paměti v šifrované podobě
       (zrychlení a částečná ochrana před hackery :-) */
    /*
    #if HRA_SLOVA_SIF_ZAP == 1
      if (!sifrovani_slov(0, HRA_SLOVA_SIF_KEY, slovo, sizeof(slovo))) {
        vymaz_obr();
        fputs(ERR_SIGN "Nacitane slovo nelze desifrovat...\n", stderr);
        nacteno = false;
        ret_val = false;
        goto f_nacti_slova_konec;
      }
    #endif
    */

    if (slova_duplicita(slovo, slova, slova_size)) {
      fprintf(stderr, ERR_SIGN "Slovo \"%s\" se ve slovniku opakuje...\n", slovo);
      pocet_slov--;
      continue;
    }

    /* příliš malý počet načtených slov */
    if (pocet_slov < pocet_slov_min) {
      vymaz_obr();
      fprintf(
        stderr
        , ERR_SIGN "Min. pocet slov ve slovniku je %d,\n"
          "    dostupnych aktualne max. %d slov...\n"
        , HRA_POCETSLOV_MIN
        , pocet_slov
      );
      nacteno = false;
      ret_val = false;
      goto f_nacti_slova_konec;
    }

    /* začlenění načteného slova do hádaných slov */

    /* pokus o alokaci paměti pro slovo */
    if (!(slova[i] = (char *) malloc(strlen(slovo) + 1))) {
      vymaz_obr();
      fputs(ERR_SIGN "Nedostatek volne pameti - slova nenactena...\n", stderr);
      nacteno = false;
      ret_val = false;
      goto f_nacti_slova_konec;
    }
    else {
      /* zkopírování načteného slova do pole hádaných slov */
      strcpy(slova[i], slovo);
      i++;
    }
  }

  /* příliš malý počet načtených slov - kontrola posledního řádku */
  if (pocet_slov < pocet_slov_min) {
    vymaz_obr();
    fprintf(
      stderr
      , ERR_SIGN "Min. pocet slov ve slovniku je %d,\n"
        "    dostupnych aktualne max. %d slov...\n"
      , HRA_POCETSLOV_MIN
      , pocet_slov
    );
    nacteno = false;
    ret_val = false;
    goto f_nacti_slova_konec;
  }

  /* příp. redukce alok. paměti dle počtu skutečně načtených slov */
  slova_size = i;  /* i když se realloc() nezdaří, zbytek pole je NULL */
  if (!(pp_char = (char **) realloc(slova, sizeof(char *) * slova_size))) {
    fputs(ERR_SIGN "Nepodarilo se zmensit alokovanou pamet...\n", stderr);
  }
  else {
    slova = pp_char;
    pp_char = NULL;
  }

  f_nacti_slova_konec:

  /* uzavře soubor se slovy */
  if (f_slova) {
    if (fclose(f_slova) == EOF) {
      vymaz_obr();
      fputs(ERR_SIGN "Nelze zavrit soubor se slovy...\n", stderr);
    }
  }

  slova_nactena = nacteno;
  return ret_val;
}

static int hra_kolo(void) {

  int i           = 0;     /* univerzální iterátor */
  int hadany_znak = '\0';  /* uživatelem zadaný znak */


  while (hra_probiha) {

   if (term_color_zap) {
      if (zbyva_zivotu > UKAZATELSIBE_ZIVOT_LOW) {
        /* výchozí barva - průběh hry */
        term_barvy(TERM_POZADI, TERM_POPREDI);
      }
      else {
        /* zbývá málo životů */
        term_barvy(TERM_RED, TERM_LWHITE);
      }
    }

    vymaz_obr();
    ukazatele_vykresli();

    /* ukončení hry při nedostatku životů */
    if (zbyva_zivotu < 1) {
      hra_probiha = false;
      break;
    }
    else if (!ukazatelslov_hotovo()) {
      /* načtení znaku od uživatele */
      printf("Hadej pismeno (%c) > ", (char) VOLBA_VOLBY);
      hadany_znak = getchar();
      cekej_enter();  /* vyprázdnění vstupního bufferu */

      /* zjištění, zda nejde o vyhrazený znak (zvláštní volbu) */
      switch ((ZVLASTNI_VOLBY) hadany_znak) {

        /* ukončí probíhající hru */
        case VOLBA_KONEC:
          printf(ansi_format(ANSI_INVER) "%c%c%c Ukoncit probihajici hru?" ansi_format(ANSI_RESET) "  (a/n) >  "
                 , (int) HRA_VOLBY_ZAVLP[0]
                 , (char) VOLBA_KONEC
                 , (int) HRA_VOLBY_ZAVLP[1]);

           if (tolower(getchar()) == HRA_VOLBY_ANO) {
             while (getchar() != '\n')
               ;  /* vyprázdnění bufferu */
             return (zbyva_zivotu = 0);  /* ukončení hry */
           }
           else {
             while (getchar() != '\n')
               ;  /* vyprázdnění bufferu */
             continue;  /* pokračování hry při zamítnutí volby */
           }

        break;

        /* zobrazí dostupné volby */
        case VOLBA_VOLBY:
          fputs(">  " ansi_format(ANSI_INVER), stdout);
          for (i = 0; i < (int) (sizeof(volby_seznam) / sizeof(volby_seznam[0])); i++) {
            printf("%c%c%c %s%s"
                   , (int) HRA_VOLBY_ZAVLP[0], volby_hodnoty[i], (int) HRA_VOLBY_ZAVLP[1]
                   , volby_seznam[i]
                   , (i < (int) ((sizeof(volby_seznam) / sizeof(volby_seznam[0])) - 1)) ? HRA_VOLBY_SEP : "");
          }
          /* odstraní oddělovač položek na konci seznamu */
          /*
          for (i = 0; i < (int) strlen(HRA_VOLBY_SEP); i++) {
            putchar('\b');
          }
          */
          fputs(ansi_format(ANSI_RESET) "\n   " HRA_PROPOKRACOVANI, stdout);
          cekej_enter();
          continue;
        break;

        /* zobrazí velkou nápovědu */
        case VOLBA_NAPOVEDA:
          napoveda();
          continue;
        break;

        /* doplní písmeno za cenu určitého počtu bodů */
        case VOLBA_POMOC_ZN:
          /* zatím není implementováno */
          /*
          printf(ansi_format(ANSI_INVER) "%c%c%c Napovi znak za cenu %d b." ansi_format(ANSI_RESET) "  " HRA_PROPOKRACOVANI
                 , (int) HRA_VOLBY_ZAVLP[0]
                 , (char) VOLBA_POMOC_ZN
                 , (int) HRA_VOLBY_ZAVLP[1]
                 , HRA_POMOC_ZN_CENA);
          cekej_enter();
          continue;
          */
        break;

        default:
          break;

      }

      /* další zpracování hádaného znaku a vyhodnocení úspěšnosti */
      if (ukazatelpismen_vydej(hadany_znak)) {
        if (ukazatelslov_hadej(hadany_znak) < 1) {
          ukazatelsibenice_nastav(--zbyva_zivotu, celkem_bodu);
        }
      }
      else {
        ukazatelslov_hlaska("");
        fputs(ansi_format(ANSI_INVER) HRA_ZNAK_NEDOSTUPNY ansi_format(ANSI_RESET) "  " HRA_PROPOKRACOVANI, stdout);
        cekej_enter();  /* čekání na stisk klávesy enter */
      }
    }
    else {
      /* slovo jsi uhodl */
      #if (ZVUKY == 1)
        putchar('\a');
      #endif
      if (term_color_zap) {
        vymaz_obr();  /* POKUS o eliminaci probliknutí obrazovky */
        term_barvy(TERM_GREEN, TERM_LWHITE);
        vymaz_obr();
        ukazatele_vykresli();
      }
      fputs(ansi_format(ANSI_INVER) HRA_HLASKA_UHODL ansi_format(ANSI_RESET) "  " HRA_PROPOKRACOVANI, stdout);
      cekej_enter();  /* čekání na stisk klávesy enter */
      break;
    }

  }

  return zbyva_zivotu;
}


/* return: true => [slovo] se vyskytuje v [seznam] */
static bool slova_duplicita(char *slovo, char **seznam, int seznam_size)
{
  register int i;

  /* kontrola argumentů */
  if ((!slovo) || (!seznam) || (seznam_size <= 0))  return false;

  /* hledání slova v poli */
  for (i = 0; i < seznam_size; i++) {
    if (seznam[i] == NULL) {
      continue;
    }
    else if (strcmp(slovo, seznam[i]) == 0) {
      return true;
    }
  }

  return false;
}
