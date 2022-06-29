#include "konfigurace.h"
#include "ukazatel_slov.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>

static int  i, j      = 0;  /* pomocný iterátor */
static bool nastaveno = false;  /* určuje, zda je modul inicializován */

typedef struct {
  char slovo[UKAZATELSLOV_DELKA_MAX + 1];
  bool odkryto[UKAZATELSLOV_DELKA_MAX + 1];
  int  delka;
  int  zbyva;
} HADANE_SLOVO;
static HADANE_SLOVO hadane_slovo = { .slovo[0] = '\0', .odkryto[0] = '\0', .delka = 0, .zbyva = 0 };


static char *hlasky_ano[] = { UKAZATELSLOV_HLASKY_ANO };
static char *hlasky_ne [] = { UKAZATELSLOV_HLASKY_NE  };
static char hlaska[UKAZATELSLOV_HLASKA_MAX] = "";
static int  pocet_hlasek_ano  = 0;
static int  pocet_hlasek_ne   = 0;
static int  stav_nalezeno     = (-1);


void ukazatelslov_nastav(char slovo[])
{
  if (strlen(slovo) > 0 && strlen(slovo) <= UKAZATELSLOV_DELKA_MAX) {

    pocet_hlasek_ano = sizeof(hlasky_ano) / sizeof(hlasky_ano[0]);
    pocet_hlasek_ne  = sizeof(hlasky_ne) / sizeof(hlasky_ne[0]);
    
    strcpy(hadane_slovo.slovo, slovo);
    for (i = 0; i < (int) (sizeof(hadane_slovo.odkryto) / sizeof(hadane_slovo.odkryto[0])); i++) {
      hadane_slovo.odkryto[i] = false;
    }
    hadane_slovo.delka = strlen(slovo);
    hadane_slovo.zbyva = hadane_slovo.delka;

    for (i = 0; i < hadane_slovo.delka; i++) {
      for (j = 0; j < (int) strlen(UKAZATELSLOV_PRESKOCIT); j++) {
        if (hadane_slovo.slovo[i] == UKAZATELSLOV_PRESKOCIT[j]) {
          hadane_slovo.odkryto[i] = true;
          hadane_slovo.zbyva--;
          break;
        }
      }
    }

    nastaveno = true;
  }
  else {
    nastaveno = false;
    fputs(ERR_SIGN "Modul ukazatele slov nebyl inicializovan...\n", stderr);
  }
}


int ukazatelslov_hadej(int znak)
{
  if (nastaveno) {
    
    int nalezeno = 0;
    int odecist  = 0;
    
    /* hledání vyskytů znaku ve slově */
    for (i = 0; i < hadane_slovo.delka; i++) {
      if (toupper(hadane_slovo.slovo[i]) == toupper(znak)) {
        hadane_slovo.odkryto[i] = true;
        nalezeno++;
        odecist++;

        /* (!) NEFUNGUJE SPRÁVNĚ - VYPNUTO V KONFIGURACI */
        /* při zvolení písmene C nebo H odkryje i výskyty CH */
        #if (UKAZATELSLOV_PISMENO_CH == 1)
          if (toupper(znak) == 'C') {
            if (i < hadane_slovo.delka - 1) {
              if (toupper(hadane_slovo.slovo[i + 1]) == 'H') {
                hadane_slovo.odkryto[i + 1] = true;
                odecist++;
              }
            }
          }
          else if (toupper(znak) == 'H') {
            if (i > 0) {
              if (toupper(hadane_slovo.slovo[i - 1]) == 'C') {
                hadane_slovo.odkryto[i - 1] = true;
                odecist++;
              }
            }
          }
        #endif

        if (hadane_slovo.zbyva == 0)  break;
      }
    }
    
    hadane_slovo.zbyva -= (hadane_slovo.zbyva > 0) ? odecist : 0;
    stav_nalezeno = (nalezeno > 0) ? 1 : 0;

    return nalezeno;

  }
  else {
    fputs(ERR_SIGN "Modul ukazatele slov nebyl inicializovan...\n", stderr);
    stav_nalezeno = (-1);
    return (-1);
  }
}

void ukazatelslov_hlaska(char *retezec)
{
  if (strlen(retezec) < UKAZATELSLOV_HLASKA_MAX) {
    strncpy(hlaska, (strlen(retezec) < 1) ? " " : retezec, sizeof(hlaska));
  }
  else {
    fputs(ERR_SIGN "Hlaska je prilis dlouha...\n", stderr);
  }

  stav_nalezeno = -1;
}

void ukazatelslov_vykresli(void)
{
  int pocet_mezer = ((UKAZATELE_SIRKA_BUNKY + 1) * POCET_KOL) - 1;

  if (nastaveno) {

    /* přednastavení náhodné hlášky */
    if (stav_nalezeno == 1) {
      ukazatelslov_hlaska(hlasky_ano[rand() % pocet_hlasek_ano]);
    }
    else if (stav_nalezeno == 0) {
      ukazatelslov_hlaska(hlasky_ne[rand() % pocet_hlasek_ne]);
    }

    /* první řádek */

    putchar('|');
    for (i = 0; i < pocet_mezer; i++) {
      if (i == pocet_mezer - (UKAZATELE_SIRKA_BUNKY * 2) - 2)
        putchar('|');
      else
        putchar(' ');
    }
    puts("|");

    /* druhý řádek */

    fputs("|     " UKAZATELSLOV_PROMPT, stdout);
    /* vypíše hádanku */
    for (i = 0; i < hadane_slovo.delka; i++) {
      if (hadane_slovo.odkryto[i])
        putchar((UKAZATELSLOV_VELKA) ? toupper(hadane_slovo.slovo[i]) : hadane_slovo.slovo[i]);
      else
        putchar(UKAZATELSLOV_MASKA);
    }
    /* vypíše rámeček s hláškou */
    for (i = 0; i < (int) (pocet_mezer - 5 - strlen(UKAZATELSLOV_PROMPT) - hadane_slovo.delka); i++) {
      if ((int) (i + 5 + strlen(UKAZATELSLOV_PROMPT) + hadane_slovo.delka) == pocet_mezer - (UKAZATELE_SIRKA_BUNKY * 2) - 2) {
        putchar('|');
        for (j = 0; j < (int) ((((UKAZATELE_SIRKA_BUNKY * 2) - strlen(hlaska)) / 2) + 1); j++) {
          putchar(' ');
        }
        fputs(hlaska, stdout);
        for (j = 0; j < (int) ((((UKAZATELE_SIRKA_BUNKY * 2) - strlen(hlaska)) / 2) + 1); j++) {
          putchar(' ');
        }
        break;
      }
      else
        putchar(' ');
    }
    puts("|");

    /* třetí řádek */

    putchar('|');
    for (i = 0; i < pocet_mezer; i++) {
      if (i == pocet_mezer - (UKAZATELE_SIRKA_BUNKY * 2) - 2)
        putchar('|');
      else
        putchar(' ');
    }
    puts("|");
  }
  else {
    fputs(ERR_SIGN "Modul ukazatele slov nebyl inicializovan...\n", stderr);
  }
}


int ukazatelslov_delka(void)
{
  if (nastaveno) {
    return hadane_slovo.delka;
  }
  else {
    fputs(ERR_SIGN "Modul ukazatele slov nebyl inicializovan...\n", stderr);
    return 0;
  }
}


int ukazatelslov_zbyva(void)
{
  if (nastaveno) {
    return hadane_slovo.zbyva;
  }
  else {
    fputs(ERR_SIGN "Modul ukazatele slov nebyl inicializovan...\n", stderr);
    return 0;
  }
}

void ukazatelslov_odkryj(void)
{
  register int i = 0;

  if (nastaveno) {
    for (i = 0; i < hadane_slovo.delka; i++) {
      hadane_slovo.odkryto[i] = true;
    }
    for (i = 0; i < hadane_slovo.delka; i++) {
      hadane_slovo.slovo[i] = toupper(hadane_slovo.slovo[i]);
    }
  }
}

bool ukazatelslov_hotovo(void) {
  if (nastaveno) {
    if (hadane_slovo.zbyva <= 0) {
      return true;
    }
    else {
      return false;
    }
  }
  else {
    fputs(ERR_SIGN "Modul ukazatele slov nebyl inicializovan...\n", stderr);
    return false;
  }
}
