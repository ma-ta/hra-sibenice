/* KÓD NENÍ BEZ CHYB, nicméně funkční pro omezený rozsah
   predikovatelných vstupních hodnot (v rámci aplikace) */

#include <stdio.h>
#include "../globconf.h"
#include "krypto.h"


/* TEST MODULU - aktivace fce. main()
   (definováno také v globconf.h)  */
/***********************************/
#undef  DEBUG
#define DEBUG  0       /* VYP => 0 */
/***********************************/
#if DEBUG == 1
  #include <string.h>
#endif
/***********************************/


/* klasická Caesarova šifra (zvážena i XOR šifra, kde je problém s uložením
   výsledných bajtů (znaků) do textového souboru -> řešením je zápis v HEX
   (délka zašifrovaného a odšifrovaného slova se poté liší) */
bool sifrovani_slov(int rezim, int klic, char *buffer, int buffer_size)
{
  char *p_znak = buffer;  /* zpracovávaný znak */
  int posun;  /* šifrovací konstanta (k přičtení/odečtení) */
  int dolni_znak;  /* hodnota prvního znaku ('a', 'A', '0') */
  int rozsah;  /* poslední znak ('z', 'Z', '9') */
  int znak;  /* kvůli dereferencování (pro pohodlnost) */
  bool ret_val = true;  /* true: slovo úspěšně odšifrováno */

  if (buffer == NULL) {
    ret_val = false;
    goto f_sifrovani_slov_konec;
  }

  /* nastavení režimu funce dle parametru */
  switch (rezim) {
    case 0:  /* odšifrování */
      posun = (-1) * klic;
      break;
    case 1:  /* zašifrování */
      posun = klic;
      break;
    default:
      fprintf(stderr, ERR_SIGN "%s(): chybna hodnota arg: rezim", __func__);
      ret_val = false;
      goto f_sifrovani_slov_konec;
  }

  /* výkonná část */

  while ((p_znak < (buffer + buffer_size)) && *p_znak != '\0') {
    znak = (int) *p_znak;

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

  f_sifrovani_slov_konec:

  return ret_val;
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
  FILE *f_vstup = NULL;

  if (argc != 3)  return 1;

  if (argv[1][0] != '0' && argv[1][0] != '1') {
    fprintf(stderr, ERR_SIGN "%s(): argv[1] == 1 && 0 !\n", __func__);
    return 1;
  }
  rezim = (int) (argv[1][0] - '0');

  if ((f_vstup = fopen(argv[2], "r")) == NULL) {
    fprintf(stderr, ERR_SIGN "%s(): soubor \"%s\" nelze otevrit\n", __func__, argv[2]);
    return 1;
  }

  while (fgets(buffer, sizeof(buffer), f_vstup)) {
    if ((p_char = strchr(buffer, '\r')))  *p_char = '\0';
    if ((p_char = strchr(buffer, '\n')))  *p_char = '\0';

    if (!sifrovani_slov(rezim, SIFROVACI_KLIC, buffer, sizeof(buffer))) {
      fputs(ERR_SIGN "Nacitane slovo nelze desifrovat...\n", stderr);
      goto f_main_krypto_konec;
    }
    printf("%s\n", buffer);
  }

  f_main_krypto_konec:

  if (f_vstup) {
    if (fclose(f_vstup) == EOF) {
      fputs(ERR_SIGN "Nelze zavrit soubor se slovy...\n", stderr);
    }
  }

  return 0;
}

#endif  /* test modulu */
