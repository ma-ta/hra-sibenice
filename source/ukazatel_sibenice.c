#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "ansi_format.h"
#include "konfigurace.h"

/* zbývající životy */
static int zbyvajici_zivoty = 0;
/* počet získaných bodů */
static int celkove_body     = 0;
/* pole s obrázky šibenice */
static char *sibenice_obr[POCET_ZIVOTU + 1];
/* informuje, zda je modul inicializován */
static bool nastaveno       = false;


/* soukromé funkce */

bool ukazatelsibenice_nahrajobr(void)
{
  FILE *f_sibenice = NULL;
  char nactena_cast[10000] = "";
  int cislo_obrazku = POCET_ZIVOTU;
  int c = 0;
  char s_c[2] = "";

  if ((f_sibenice = fopen(UKAZATELSIBENICE_SOUBOR, "r")) == NULL) {
    fprintf(stderr, ERR_SIGN ERR_SOUBOR "\n", UKAZATELSIBENICE_SOUBOR);
    return false;
  }

  while ((c = getc(f_sibenice)) != EOF && cislo_obrazku < (int) (sizeof(sibenice_obr) / sizeof(sibenice_obr[0]))) {
    if (c == UKAZATELSIBENICE_ZARAZKA) {
      if ((sibenice_obr[cislo_obrazku] = (char *) malloc(strlen(nactena_cast) + 1)) == NULL) {
        fputs(ERR_SIGN "Nedostatek pameti pro obrazky...\n", stderr);
        return false;
      }
      strcpy(sibenice_obr[cislo_obrazku], nactena_cast);
      cislo_obrazku--;
      *nactena_cast = '\0';

      while ((c = getc(f_sibenice)) != '\n' && c != EOF)
        ;
    }
    else {
      *s_c = c;
      strcat(nactena_cast, s_c);
    }
  }

  if (fclose(f_sibenice) == EOF) {
    fputs(ERR_SIGN "Soubor s obrazky sibenice nelze zavrit...\n", stderr);
  }


  return true;
}


/* veřejné funkce */

void ukazatelsibenice_vycisti(void)
{
  register int i = 0;

  for (i = 0; i < (int) (sizeof(sibenice_obr) / sizeof(sibenice_obr[0])); i++) {
    free((void *) sibenice_obr[i]);
  }
}

void ukazatelsibenice_nastav(int zivoty, int body)
{
  zbyvajici_zivoty = zivoty;
  celkove_body     = body;

  if (!nastaveno) {
    if (!ukazatelsibenice_nahrajobr()) {
      nastaveno = false;
      return;
    }   
  }

  nastaveno = true;
}

int ukazatelsibenice_zjisti_zivoty(void) {
  return zbyvajici_zivoty;
}

int ukazatelsibenice_zjisti_body(void) {
  return celkove_body;
}


/* vykreslování šibenice */


void ukazatelsibenice_vykresli(void) {
  if (nastaveno) {

   printf((const char *) sibenice_obr[zbyvajici_zivoty]
          , CSI ANSI_ULINE SGR
          , CSI ANSI_RESET SGR
          , CSI ANSI_ULINE SGR
          , zbyvajici_zivoty
          , CSI ANSI_RESET SGR
          , celkove_body);
  }
  else {
    fputs(ERR_SIGN "Modul sibenice neni spravne inicializovan...\n\n", stderr);
    
    puts  ("+-----------------+");
    printf("|  ZIVOTY  :  %02d  |\n+-----------------+\n|  BODY    :  %02d  |\n",
           zbyvajici_zivoty, celkove_body);
    puts  ("+-----------------+");
  }
}
