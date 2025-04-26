/*
 *  Obsahuje funkce pro šifrování a dešifrování
 *  položek uložených ve slovníku hádaných slov
 *
 */

#ifndef KRYPTO
#define KRYPTO


/* rezim: 1 - zašifrovat, 0 - odšifrovat;
   klic: heslo (apod.) pro zašifrování, odšifrování;
   buffer: pro vstupní slovo i zápis výsledku */
char *sifrovani_slov(int rezim, int klic, char *buffer);


#endif
