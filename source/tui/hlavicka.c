#include <stdio.h>
#include "../globconf.h"

static const char *hlavicka = \
"+-----------------------------------------------------+\n"
"|                                                     |\n"
"|               H R A                                 |\n"
"|                       S I B E N I C E               |\n"
"|                                                     |\n"
"+--------------------------+--------------------------+\n"
"|      (c) 2022 Ma-TA      |        ver. %s        |\n"
"+--------------------------+--------------------------+\n";


void hlavicka_vykresli(void)  {
  printf(hlavicka, VERZE);
}
