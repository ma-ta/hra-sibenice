/*
 * hlavičkový soubor panelu
 * s dostupnými písmeny
 */

#ifndef UKAZATEL_PISMEN_H
#define UKAZATEL_PISMEN_H


/* inicializuje ukazatel
   dostupnými znaky */
void ukazatelpismen_nastav
(int delka_radku, int sirka_bunky);

/* požádá o vydání znaku ze seznamu
   vrátí výsledek dotazu */
bool ukazatelpismen_vydej
(int znak);

/* vykreslí ukazatel písmen
   na standardní výstup */
void ukazatelpismen_vykresli(void);


#endif
