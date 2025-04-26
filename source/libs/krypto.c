#include <stdio.h>
#include "../globconf.h"
#include "krypto.h"


// TEST MODULU - aktivace fce. main()
// (definováno také v globconf.h)
//===========================
#undef  DEBUG
#define DEBUG  0  // VYP => 0
//===========================
#if DEBUG == 1
  #include <string.h>
#endif
//===========================


/* klasická Caesarova šifra
   (zvážena i XOR šifra, kde je problém s uložením výsledných bajtů (znaků)
    do textového souboru -> řešením je zápis v HEX, ale délka slova se poté
    liší - nutno přepsat načítání slov a alokaci paměti v hra.c) */
char *sifrovani_slov(int rezim, int klic, char *buffer)
{
  if (!buffer)  return NULL;

  char *p_znak = buffer;  /* zpracovávaný znak */
  int posun;  /* šifrovací konstanta (k přičtení/odečtení) */

  /* nastavení režimu funce dle parametru */
  switch (rezim) {
    case 0:  /* odšifrování */
      posun = (-1) * klic;
      break;
    case 1:  /* zašifrování */
      posun = klic;
      break;
    default:
      fprintf(stderr, "%s(): chybna hodnota arg: rezim", __func__);
      return NULL;
  }

  /* výkonná část */

  while (*p_znak != '\0') {
    int dolni_znak;  /* hodnota prvního znaku ('a', 'A', '0') */
    int rozsah;  /* poslední znak ('z', 'Z', '9') */
    int znak = *p_znak;  /* kvůli dereferencování (pro pohodlnost) */

    /* zpracovat či přeskočit znak */
    if (znak >= 'a' && znak <= 'z') {
      dolni_znak = 'a';
      rozsah = 'z' - dolni_znak;
    }
    else if (znak >= 'A' && znak <= 'Z') {
      dolni_znak = 'A';
      rozsah = 'Z' - dolni_znak;
    }
    else if (znak >= '0' && znak <= '9') {
      dolni_znak = '0';
      rozsah = '9' - dolni_znak;
    }
    else {
      p_znak++;
      continue;
    }

    znak -= dolni_znak;
    znak = (znak + posun + rozsah + 1) % (rozsah + 1);
    znak += dolni_znak;

    *p_znak = znak;
    p_znak++;
  }  /* while */

  return buffer;
}  /* sifrovani_slov() */




/* test modulu - příp. šifrování/odšifrování souboru
   (argumenty: ./krypto MÓD CESTA_K_SOUBORU) */
#if DEBUG == 1

#define SIFROVACI_KLIC  9
#define BUFFER_SIZE     100

int main(int argc, char *argv[])
{
  char buffer[BUFFER_SIZE];
  char *p_char = NULL;
  int rezim;

  if (argc != 3)  return 1;

  if (argv[1][0] != '0' && argv[1][0] != '1') {
    fprintf(stderr, "%s(): argv[1] == 1 && 0 !\n", __func__);
    return 1;
  }
  rezim = (int) (argv[1][0] - '0');

  FILE *f_vstup = NULL;
  if ((f_vstup = fopen(argv[2], "r")) == NULL) {
    fprintf(stderr, "%s(): soubor \"%s\" nelze otevrit\n", __func__, argv[2]);
    return 1;
  }

  while (fgets(buffer, sizeof(buffer), f_vstup)) {
    if ((p_char = strchr(buffer, '\r')))  *p_char = '\0';
    if ((p_char = strchr(buffer, '\n')))  *p_char = '\0';

    sifrovani_slov(rezim, SIFROVACI_KLIC, buffer);
    printf("%s\n", buffer);
  }

  fclose(f_vstup);

  return 0;
}

#endif  /* test modulu */
