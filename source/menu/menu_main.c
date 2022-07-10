#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "../globconf.h"
#include "./menu_main.h"
#include "../tui/hlavicka.h"

static char *polozky_menu[] = { MENU_POLOZKY };
static int i;  /* pomocné iterátory */

VOLBY_MENU menu(void)
{
  const char *NADPIS_MENU = "M E N U";
  VOLBY_MENU volba = -1;
  char volba_s[3] = "";
  int pocet_voleb = sizeof(polozky_menu) / sizeof(polozky_menu[0]);


  hlavicka_vykresli();
  putchar('\n');

  /* vypíše položky menu */

  /* první řádek */
  menu_oramuj(UKAZATELE_ORAMOVANI_KRIZENI_ZN, UKAZATELE_ORAMOVANI_ZN, MENU_SIRKA);
  putchar('|');
  for (i = 0; i < (int) ((MENU_SIRKA - strlen(NADPIS_MENU) - 1) / 2); i++)  putchar(' ');
  fputs(NADPIS_MENU, stdout);
  for (i = 0; i < (int) ((MENU_SIRKA - strlen(NADPIS_MENU) - 1) / 2); i++)  putchar(' ');
  puts("|");
  menu_oramuj(UKAZATELE_ORAMOVANI_KRIZENI_ZN, UKAZATELE_ORAMOVANI_ZN, MENU_SIRKA);
  /* vypsání položek */
  for (i = 0; i < pocet_voleb; i++) {
    menu_oramuj('|', ' ', MENU_SIRKA);
    menu_polozka(i + 1, polozky_menu[i]);
  }
  /* poslední dva řádky tabulky s položkami menu */
  menu_oramuj('|', ' ', MENU_SIRKA);
  menu_oramuj('+', '-', MENU_SIRKA);
  putchar('\n');
  
  /* načtení volby od uživatele */

  while (volba < (VOLBY_MENU) 1 || volba > (VOLBY_MENU) pocet_voleb) {
    fputs("    [   ]   <  Zadejte cislo polozky", stdout);
    for (i = 0; i < 30; i++)  putchar('\b');
    scanf("%2s", volba_s);
    cekej_enter();  /* vymazání vstupního bufferu */
    volba = (VOLBY_MENU) atoi(volba_s);
  }


  return ((volba < (VOLBY_MENU) 0 || volba > (VOLBY_MENU) pocet_voleb) ? 0 : volba);
}
