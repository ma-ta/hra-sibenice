#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../globconf.h"
#include "stats.h"


/* vynuluje stats_skore */
#define vynuluj_skore()  { int i = 0;  \
                           \
                           for (i = 0; i < STATS_POCET_HRACU; i++) {  \
                             stats_skore.jmena[i][0] = '\0';  \
                             stats_skore.skore[i] = 0;  \
                           }  \
                           data = false;  \
                         }
#define uzavri_fstats()  if (f_stats != NULL) {  \
                           if (fclose(f_stats) != EOF)  f_stats = NULL;  \
                           else fputs(ERR_SIGN "Soubor se statistikami nelze uzavrit...\n\n", stderr);  \
                         }


/* dynamické pole pro načtění šablony info obrazovky */
static char *stats_obr_sablona = NULL;
static size_t stats_obr_sablona_size = 0;
/* datový soubor se statistikami */
static FILE *f_stats = NULL;
/* určuje, zda je modul inicializovaný */
static bool nastaveno = false;
/* určuje, zda se podařilo načíst statistiky */
static bool nacteno = false;
/* určuje, zda je struktura prázdná nebo ne */
static bool data = false;

/* datové struktury statistik */
typedef char JMENO[STATS_JMENO_STRLN + 1];
struct stats_skore {
  int   skore[STATS_POCET_HRACU];
  JMENO jmena[STATS_POCET_HRACU];
} stats_skore;
struct stats_cas {
  time_t aktualni_cas;
  time_t celkovy_cas;
} stats_cas;


/* soukromé funkce modulu */

bool nahraj_sablonu(void)
{
  /* textový soubor s šablonou obrazovky statistik */
  FILE   *f_sablona        = NULL;
  size_t potrebna_velikost = 0;
  int    zkopirovano_zn    = 0,
         c                 = '\0';

  /* otevření souboru */
  if ((f_sablona = fopen(STATISTIKY_OBRSAB_SOUBOR, "r")) == NULL) {
    fprintf(stderr,
            ERR_SIGN "Nelze otevrit sablonu obrazovky statistik:\n"
            "    %s"
            , STATISTIKY_OBRSAB_SOUBOR);
    return false;
  }

  /* zjištění potřebného místa k alokování */
  while (getc(f_sablona) != EOF)  potrebna_velikost++;
  stats_obr_sablona_size = (++potrebna_velikost);  /* ukončovací znak '\0' */
  fseek(f_sablona, 0, SEEK_SET);
  /* pokus o alokování paměti */
  if ((stats_obr_sablona = (char *) malloc(potrebna_velikost)) == NULL) {
    puts(ERR_SIGN "Nedostatek pameti pro sablonu obrazovky statistik...\n\n");
    fclose(f_sablona);  f_sablona = NULL;
    return false;
  }

  /* zkopírování obsahu souboru do paměti */
  while ((c = getc(f_sablona)) != EOF && (size_t) zkopirovano_zn < potrebna_velikost) {
    *(stats_obr_sablona + (zkopirovano_zn++)) = c;
  }
  stats_obr_sablona[zkopirovano_zn] = '\0';

  /* uzavření souboru */
  if (f_sablona != NULL) {
    fclose(f_sablona);  f_sablona = NULL;
  }

  return true;
}

/* zapíše ověřovací data do souboru
   a vrátí zapsaný kontrolní součet */
int zapis_pravost_f(void)
{
  #if STATS_PRAVOST_ZAP
    
  #else
    return 0;
  #endif
}

/* ověří pravost souboru se statistikami
   pomocí otisku souboru a tajného čísla */
bool over_pravost_f(void)
{
  #if STATS_PRAVOST_ZAP

  #else
    return true;
  #endif
}


/* veřejné funkce z stats.h */


bool stats_nastav(void)
{
  if (!nastaveno) {

    /* soubor se statistikami se nepodařilo otevřít */
    if ((f_stats = fopen(STATISTIKY_SOUBOR, "rb")) == NULL) {
      fprintf(stderr,
              ERR_SIGN "Soubor se statistikami nenalezen:\n"
              "    %s\n\n",
              STATISTIKY_SOUBOR);
      /* nastaví strukturu stats_skore na nulové hodnoty */
      vynuluj_skore();
      nastaveno = true;
      return (nacteno = data = false);
    }
    /* soubor se statistikami otevřen - načtení dat */
    else {
      /* načtení skóre */
      if (fread(stats_skore.skore, sizeof(stats_skore.skore), 1, f_stats) != 1) {
        /* chyba čtení dat */
        fprintf(stderr,
                ERR_SIGN "Chyba pri nacitani statistik ze souboru (body)...\n\n");
        vynuluj_skore();
        uzavri_fstats();
        nastaveno = true;
        return (nacteno = data = false);
      }
      /* načtení jmen hráčů */
      if (fread(stats_skore.jmena, sizeof(stats_skore.jmena), 1, f_stats) != 1) {
        /* chyba čtení dat */
        fprintf(stderr,
                ERR_SIGN "Chyba pri nacitani statistik ze souboru (jmena)...\n\n");
        vynuluj_skore();
        uzavri_fstats();
        nastaveno = true;
        return (nacteno = data = false);
      }
    }

    uzavri_fstats();

    /* načtení šablony obrazovky statistik */
    (void) nahraj_sablonu();

    nastaveno = true;
    data = (stats_skore.skore[0] > 0) ? true : false;
    return (nacteno = true);
  }

  /* modul je již nastaven */
  else {
    return false;
  }
}

bool stats_nastaveno(void)  { return nastaveno; }
bool stats_nacteno(void)    { return nacteno; }
bool stats_data(void)       { return data; }

bool stats_uloz(void)
{
  if (nastaveno) {
    /* otevření souboru */
    if (f_stats != NULL)  fclose(f_stats);
    if ((f_stats = fopen(STATISTIKY_SOUBOR, "wb")) == NULL) {
      /* chyba zápisu dat */
      fprintf(stderr,
              ERR_SIGN "Nelze otevrit soubor pro zapis:\n"
              "    " STATISTIKY_SOUBOR "\n\n");
      return false;
    }

    /* uložení bodů */
    if (fwrite(stats_skore.skore, sizeof(stats_skore.skore), 1, f_stats) != 1) {
      fprintf(stderr,
              ERR_SIGN "Chyba pri ukladani statistik (body)...\n\n");
      uzavri_fstats();
      return false;
    }
    /* uložení jmen hráčů */
    if (fwrite(stats_skore.jmena, sizeof(stats_skore.jmena), 1, f_stats) != 1) {
      fprintf(stderr,
              ERR_SIGN "Chyba pri ukladani statistik (jmena)...\n\n");
      uzavri_fstats();
      return false;
    }

    uzavri_fstats();
    return true;
  }
  else {
    fputs(ERR_SIGN "Modul statistik neni nastaven...\n\n", stderr);
    return false;
  }
}

int stats_zpracuj_body(int body)
{
  int i, j;  /* iterátory */

  if (nastaveno) {
    data = true;
    for (i = 0; i < arrlen(stats_skore.skore); i++) {
      /* předané body jsou v rámci TOP n */
      if (stats_skore.skore[i] <= body) {
        /* posunutí stávajících bodů a jmen dolů a zápis aktuálního skóre */
        if (stats_skore.skore[i] < body) {
          for (j = arrlen(stats_skore.skore) - 1 ; j > i; j--) {
            stats_skore.skore[j] = stats_skore.skore[j - 1];
            strcpy(stats_skore.jmena[j], stats_skore.jmena[j - 1]);
          }
          stats_skore.skore[i] = body;
          stats_skore.jmena[i][0] = '\0';
        }

        return (i + 1);
      }
    }
    return 0;
  }

  return (-1);
}

void stats_zadej_jmeno(int pozice)
{
  char jmeno[STATS_JMENO_STRLN + 1] = "";
  int index = pozice - 1;
  char *p_char = NULL;

  if (nastaveno) {

    /* načtení jména od uživatele */
    fgets(jmeno, sizeof(jmeno), stdin);
    /* oříznutí znaků konce řádku */
    if ((p_char = strchr(jmeno, '\r')) != NULL)  *p_char = '\0';
    if ((p_char = strchr(jmeno, '\n')) != NULL)  *p_char = '\0';

    /* uložení jména do struktury */
    if (index < arrlen(stats_skore.jmena) && index >= 0) {
      strncpy(stats_skore.jmena[index], jmeno, sizeof(stats_skore.jmena[index]));
    }
    else {
      fprintf(stderr,
              ERR_SIGN STATS_DELKA_ERR
              , STATS_JMENO_STRLN);
    }
  }
  else {
    fputs(ERR_SIGN "Modul statistik neni nastaven...\n\n", stderr);
  }
}

int stats_zpracuj_cas(struct tm *p_cas)
{
  if (nastaveno) {

  }
}

bool stats_zjisti_nejcas(int *h, int *min, int *s)
{
  if (nacteno) {

  }
}

int stats_zjisti_nte_nejbody(int nte_poradi)
{
  int index = nte_poradi - 1;

  if (nastaveno && data) {
    if (index < STATS_POCET_HRACU && index >= 0) {
      return (stats_skore.skore[index]);
    }
    else {
      fprintf(stderr,
              ERR_SIGN "Uklada se pouze 1. az %d. poradi...\n\n"
              , STATS_POCET_HRACU);
    }
  }

  return 0;
}

const char *stats_zjisti_nte_nejjmeno(int nte_poradi)
{
  int index = nte_poradi - 1;

  if (nastaveno && data) {
    if (index < STATS_POCET_HRACU && index >= 0) {
      return ((const char *) stats_skore.jmena[index]);
    }
    else {
      fprintf(stderr,
          ERR_SIGN "Uklada se pouze 1. az %d. poradi...\n\n"
          , STATS_POCET_HRACU);
    }
  }

  return "";
}

bool stats_vypis(bool jednoduchy_vypis)
{
  int i, j;
  bool jmeno_vyplneno = false;
  char vystup[1000] = "";
  char *odrazky[] = { STATS_OBR_ODRAZKY };
  int pocet_zn = 0;

  if (nastaveno && data) {
    vymaz_obr();

    /* zjednodušený výpis */

    if (jednoduchy_vypis || stats_obr_sablona == NULL) {
      for (i = 1; i < STATS_POCET_HRACU; i++) {
        jmeno_vyplneno = (strcmp(stats_zjisti_nte_nejjmeno(i), "") == 0) ? false : true;
        printf("%d.  %02d b.%s%s\n"
        , i
        , stats_zjisti_nte_nejbody(i)
        , (jmeno_vyplneno) ? " -> " : ""
        , stats_zjisti_nte_nejjmeno(i));
      }
    }

    /* obrazovka se statistikami */

    else {
      /* sestavení tabulky */
      for (i = 0; i < STATS_POCET_HRACU; i++) {
        pocet_zn += snprintf(vystup + pocet_zn, sizeof(vystup) - pocet_zn,
                             "  %s%d.  %s%s"
                             , (i == 0) ? "* " : "  "
                             , i + 1
                             , stats_skore.jmena[i]
                             , (stats_skore.jmena[i][0] == '\0')
                                  ? STATS_OBR_VODITKO STATS_OBR_VODITKO
                                  : "  ");
        for (j = 0; j < STATS_OBR_RADEK - 2 - strlen(stats_skore.jmena[i]); j++) {
          strncat(vystup + pocet_zn, STATS_OBR_VODITKO, sizeof(vystup) - pocet_zn);
          pocet_zn++;
        }
        pocet_zn += snprintf(vystup + pocet_zn, sizeof(vystup) - pocet_zn,
                             "  %s  %2d b.\n"
                             , (i < arrlen(odrazky)) ? odrazky[i] : "           "
                             , stats_skore.skore[i]);
      }

      /* výstup na obrazovku */
      puts(STATS_OBR_ZAHLAVI);
      printf((const char *) stats_obr_sablona, vystup);

    }

    return true;
  }
  else if (nastaveno && !data) {
    puts(STATS_ZADNE_STATS);
  }
  else {
    fputs(ERR_SIGN "Modul statistik neni nastaven...\n\n", stderr);
    return false;
  }
}

bool stats_vymaz(void)
{
  if (nastaveno) {
    vynuluj_skore();
    return true;
  }
  else {
    fputs(ERR_SIGN "Modul statistik neni nastaven...\n\n", stderr);
    return false;
  }
}

void stats_vycisti(void)
{
  (void) free((void *) stats_obr_sablona);
  stats_obr_sablona = NULL;
  stats_obr_sablona_size = 0;
}
