#include <stdio.h>
#include "../globconf.h"

static const char *hlavicka_obr = TUI_HLAVICKA_OBR;


void hlavicka_vykresli(void)  {
  printf(hlavicka_obr, VERZE);
}
