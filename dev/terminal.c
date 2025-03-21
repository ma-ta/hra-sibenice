/* (c) 2025  Martin TÁBOR
 *
 * Pokus o multiplatformní řešení změny velikosti a titulku
 * okna (emulátoru) terminálu
 */


#include <stdio.h>
#include <wchar.h>
#include "../source/globconf.h"
#include "../source/libs/ansi_fmt.h"
#ifdef OS_WIN
    #include <windows.h>
#endif

#define ARG_COUNT      2
#define ARG_INFO      "Argumenty:  COLS_X LINES_Y"
#define WINDOW_TITLE  "Muj titulek okna"


void term_set(int x, int y);


int main(int argc, char *argv[])
{
    int term_size[2] = { 0, 0 };


    // parsování argumentů

     if (argc != ARG_COUNT + 1) {  // počet argumentů
        fprintf(stderr, ARG_INFO "\n");
        return 1;
    }
    for (int i = 0; i < ARG_COUNT; i++) {  // načítání argumentů
        int argument = i + 1;
        if (sscanf(argv[argument], "%d\n", term_size + i) != 1) {
            fprintf(
                stderr,
                "Chyba pri nacitani %d. argumentu: \"%s\"!\n",
                i, argv[argument]
            );
            return 1;
        }
        else if (term_size[i] <= 0) {  // ověření povolených hodnot
            fprintf(stderr, "X i Y musi byt vetsi nez 0!\n");
            return 1;
        }
    }

    // nastavení velikosti terminálu

    printf(
        "Nastavuji hodnoty:\n"
        "cols_x=%d lines_y=%d\n",
        term_size[0], term_size[1]
    );

    term_set(term_size[0], term_size[1]);

    while (getchar() != '\n')
        ;
    return 0;
}


void term_set(int x, int y)
{
#if TERM_SET == 1
    #if (defined(OS_UNIX) && !defined(OS_MAC))
        printf("-- OS_UNIX --\n");

        printf(ansi_osc_title_v1("Baf"));
        printf(ansi_osc_title_kde("Haf"));

    #elif defined(OS_WIN)
        // https://learn.microsoft.com/en-us/windows/console/console-virtual-terminal-sequences
        printf("-- OS_WIN --\n");

        // OSC Esc nefungují v CMD.EXE (i když doporučené pro WT.EXE)
        SetConsoleTitle("Raf");

        // získání handle konzolového výstupu
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        if (hConsole == INVALID_HANDLE_VALUE) {
            fprintf(stderr, "nelze ziskat handle\n");
            return;
        }

        // získání informací o současném nastavení
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        if (!GetConsoleScreenBufferInfo(hConsole, &csbi)) {
            fprintf(stderr, "nelze ziskat info o terminalu\n");
            return;
        }

        // nastavení nové velikosti bufferu
        COORD newSize;  // definice nového bufferu
        newSize.X = ((x > (csbi.dwSize.X)) ? x : (csbi.dwSize.X));  // šířka bufferu
        newSize.Y = y;  // výška bufferu

        printf("newX: %d, newY: %d\n", newSize.X, newSize.Y);

        if (!SetConsoleScreenBufferSize(hConsole, newSize)) {
            fprintf(stderr, "nelze nastavit novy buffer\n");
        }

        // Změna velikosti okna podle nového bufferu
        SMALL_RECT srWindow;
        srWindow.Left = 0;
        srWindow.Top = 0;
        srWindow.Right = newSize.X - 1; // šířka okna
        srWindow.Bottom = newSize.Y - 1; // výška okna

        if (!SetConsoleWindowInfo(hConsole, TRUE, &srWindow)) {
            fprintf(stderr, "nepodarilo se zmenit velikost\n");
            return;
        }
    #endif
#endif
        return;
}
