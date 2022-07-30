#include <stdio.h>
#include <string.h>
#include "../globconf.h"

static const char *hlavicka_obr = TUI_HLAVICKA_OBR;

void hlavicka_vykresli(const char *text)  {
  
  char titulek[TUI_HLAVICKA_BUNKA + 1] = "";
  int  mezer = (TUI_HLAVICKA_BUNKA - (int) strlen(text)) / 2;
  int  i     = 0;

  /* ověření maximální délky textu */
  if (strlen(text) > TUI_HLAVICKA_BUNKA) {
    fputs(ERR_TUI_HLAVICKA_TXTMAX "\n", stderr);
    return;
  }

  for (i = 0; i < mezer; i++) {
    strcat(titulek, " ");
  }
  strcat(titulek, text);
  while (strlen(titulek) < (size_t) TUI_HLAVICKA_BUNKA) {
    strcat(titulek, " ");
  }

  /* ověření, zda šlo zarovnat titulek přesně na střed buňky */
  if ((strlen(text) % 2 == 0) != (TUI_HLAVICKA_BUNKA % 2 == 0)) {
    fputs(ERR_SIGN ERR_TUI_HLAVICKA_ZAROV "\n", stderr);
  }

  printf(hlavicka_obr, titulek);
}
