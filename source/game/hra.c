#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../globconf.h"
#include "./game_tui/game_tui.h"
#include "./hra.h"
#include "../libs/ansi_fmt.h"
#include "../tui/hlavicka.h"
#include "../help/napoveda.h"


typedef enum {
  HRA_VOLBY
} ZVLASTNI_VOLBY;
static char *volby_seznam[] = { HRA_VOLBY_SZN };  /* popisky zvláštních voleb */
static int  volby_hodnoty[] = { HRA_VOLBY_HODNOTY };
static FILE *f_slova = NULL;  /* soubor se slovy k hádání */
static char *slova[HRA_POCETSLOV];  /* dynamické pole pro uložení slov ze souboru */
bool slova_nactena = false;

static int hra_probiha  = false;
static int pocet_kol    = 0;
static int kolo_hry     = 0;
static int celkem_bodu  = 0;
static int pocet_zivotu = 0;
static int zbyva_zivotu = 0;


/* hlavičky lokálních funkcí */
int hra_kolo(void);
bool nacti_slova(void);


/* veřejné funkce */

void hra_vysledek(int skore)
{
  hlavicka_vykresli(TUI_HLAVICKA_TXT);
  puts("\n");

  fputs("   >   " HRA_HLASKA_FORMAT, stdout);
  if (skore > 0) {
    printf(HRA_HLASKA_VYHRA, skore);
    fputs(ansi_format(ANSI_RESET), stdout);
    puts("\n\n\n" HRA_OBR_VYHRA);
  }
  else {
    fputs(HRA_HLASKA_PROHRA, stdout);
    fputs(ansi_format(ANSI_RESET), stdout);
    puts("\n\n\n" HRA_OBR_PROHRA);
  }

  /* čekání na stisk klávesy Enter */
  fputs("\n\n   " HRA_PROPOKRACOVANI, stdout);
  cekej_enter();
  vymaz_obr();
}


void hra_vycisti(void)
{
  register int i = 0;

  for (i = 0; i < (int) (sizeof(slova) / sizeof(slova[0])); i++) {
    free((void *) slova[i]);
  }
}

bool nacti_slova(void)
{
  register int i = 0;
  char slovo[100] = "";
  char *p_char = NULL;

  /* ukončí funkci, pakliže jsou slova již načtena */
  if (slova_nactena)  return true;

  /* otevře soubor se slovy */
  if ((f_slova = fopen(HRA_SLOVA_SOUBOR, "r")) == NULL) {
    vymaz_obr();
    fprintf(stderr, "\n" ERR_SIGN ERR_SOUBOR "\n", HRA_SLOVA_SOUBOR);
    return false;
  }

  /* načte slova do paměti */
  for (i = 0; i < (int) (sizeof(slova) / sizeof(slova[0])); i++) {
    if (!feof(f_slova)) {
      /* načtení řádku */
      fgets(slovo, sizeof(slovo), f_slova);
      /* ukončení v případě prázdného řádku */
      if (strlen(slovo) < 1)  continue;
      /* odstranění znaku konce řádku */
      if ((p_char = strchr(slovo, '\r')) != NULL)  *p_char = '\0';
      if ((p_char = strchr(slovo, '\n')) != NULL)  *p_char = '\0';

      if ((slova[i] = (char *) malloc(strlen(slovo) + 1)) == NULL) {
        fputs(ERR_SIGN "Nedostatek volne pameti - slova nenactena...\n", stderr);
        slova_nactena = false;
        /* uzavře soubor se slovy */
        if (fclose(f_slova) == EOF) {
          fputs(ERR_SIGN "Nelze zavrit soubor se slovy...\n", stderr);
        }
        return false;
      }
      else {
        /* zkopírování načteného slova do pole slov */
        strcpy(slova[i], slovo);
      }
    }
    else {
      break;
    }

  }

  /* uzavře soubor se slovy */
  if (fclose(f_slova) == EOF) {
    fputs(ERR_SIGN "Nelze zavrit soubor se slovy...\n", stderr);
  }

  slova_nactena = true;
  return true;
}


void hra_nastav(int kol, int zivotu) {
  pocet_kol    = kol;
  kolo_hry     = 1;
  celkem_bodu  = 0;
  pocet_zivotu = zivotu;
  zbyva_zivotu = zivotu;

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
    ukazatelslov_nastav(slova[rand() % (sizeof(slova) / sizeof(slova[0]))]);
  #endif
  ukazatelpismen_nastav(pocet_kol, UKAZATELE_SIRKA_BUNKY);
  ukazatelpismen_nastav(pocet_kol, UKAZATELE_SIRKA_BUNKY);

  hra_probiha = true;
}

int hra_start(void) {
  int bodu_kolo = 0;

  ukazatelslov_hlaska(UKAZATELSLOV_HLASKA);

  while (hra_probiha && kolo_hry <= pocet_kol) {
    bodu_kolo = hra_kolo();

    if (bodu_kolo < 1) {
      celkem_bodu = 0;
      ukazatelslov_odkryj();
      vymaz_obr();
      ukazatele_vykresli();
      #if (ZVUKY == 1)
        putchar('\a');
      #endif
      fputs(ansi_format(ANSI_INVER) "Slovo jsi NEUHADL!" ansi_format(ANSI_RESET) "  " HRA_PROPOKRACOVANI, stdout);
      cekej_enter();
      vymaz_obr();
      break;
    }
    else {
      celkem_bodu += bodu_kolo;
      zbyva_zivotu = pocet_zivotu;
      kolo_hry++;
      ukazatelslov_hlaska("");
      ukazatelsibenice_nastav(pocet_zivotu, celkem_bodu);
      ukazatelkol_nastav(kolo_hry, pocet_kol, UKAZATELE_SIRKA_BUNKY);
      #if (DEBUG == 2)
        ukazatelslov_nastav(DEBUG_HADANE_SLOVO);
      #else
        ukazatelslov_nastav(slova[rand() % (sizeof(slova) / sizeof(slova[0]))]);
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


int hra_kolo(void) {

  int i           = 0;     /* univerzální iterátor */
  int hadany_znak = '\0';  /* uživatelem zadaný znak */


  while (hra_probiha) {
    
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
          printf(ansi_format(ANSI_INVER) "%c%c%c Hra byla ukoncena." ansi_format(ANSI_RESET) "  " HRA_PROPOKRACOVANI
                 , (int) HRA_VOLBY_ZAVLP[0]
                 , (char) VOLBA_KONEC
                 , (int) HRA_VOLBY_ZAVLP[1]);
          cekej_enter();
          return (zbyva_zivotu = 0);
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
        fputs(ansi_format(ANSI_INVER) "Tento znak neni k dispozici." ansi_format(ANSI_RESET) "  " HRA_PROPOKRACOVANI, stdout);
        cekej_enter();  /* čekání na stisk klávesy enter */
      }
    }
    else {
      #if (ZVUKY == 1)
        putchar('\a');
      #endif
      fputs(ansi_format(ANSI_INVER) "Slovo jsi uhadl!" ansi_format(ANSI_RESET) "  " HRA_PROPOKRACOVANI, stdout);
      cekej_enter();  /* čekání na stisk klávesy enter */
      break;
    }

  }

  return zbyva_zivotu;
}
