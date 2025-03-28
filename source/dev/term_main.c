#include "term_set.h"


int main(void)
{
  #ifdef OS_WIN
    /* zajistí otevření v okně starého Windows Console Host při současném
      použití přepínačů /link /subsystem:windows /entry:mainCRTStartup
      při kompilaci přes cl.exe */

    AllocConsole(); // otevřen nové okno konzole (ConHost.exe)
    /* přesměrování vstupů a výstupů do standardních I/O pro ConHost
      (program byl totiž spuštěn jako Windows GUI aplikace) */
    freopen("CONOUT$", "w", stdout);
    freopen("CONOUT$", "w", stderr);
    freopen("CONIN$", "r", stdin);
    /* přenesení okna terminálu do popředí (jinak je je nutné na okno
      terminálu po spuštění aplikace kliknout)
      vyžaduje připojení knihovny /link user32.lib */
    HWND hwnd = GetConsoleWindow();
    if (hwnd) {
      SetForegroundWindow(hwnd);
    }
  #endif

  term_title("Ahoj, svete!");
  term_size(TERM_SIRKA, TERM_VYSKA);
  printf("Sirka: %d\nVyska: %d\n", TERM_SIRKA, TERM_VYSKA);
  printf("(stiskni enter...)");
  while (getchar() != '\n')
    ;

  return 0;
}
