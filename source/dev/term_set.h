/*
 *  (c) 2025 - Hra Šibenice v1.1.0
 *
 *  MODUL JE TŘEBA AKTIVOVAT DEFINOVÁNÍM SYMBOLICKÉ KONSTANTY:
 *  TERM_SET  1
 *
 *  Ve Windows vyžaduje kompilaci přes cl.exe s přepínači:
 *  [/link] - následující přepínače interpretuje linker,
 *  [/subsystem:windows] - aplikace se bude spouštět jako GUI,
 *                         což zamezí případnému otevření Windows Terminal,
 *  [/entry:mainCRTStartup] - vstupním bodem aplikace zůstane main()
 *                            a zároveň budou podporovány fce. z <stdio.h>,
 *  [user32.lib] - pro volání některých fcí. WinAPI pro manipulaci s oknem
 *                 (AllocConsole() totiž není okno ConHost aktivní a kód
 *                 ho automaticky přesune do popředí, aby do něho šlo psát
 *                 bez nutnosti kliknutí myší nebo použití Alt+Tab)
 *
 *  Modul pro nastavení emulátorů terminálu (multiplatformní řešení)
 *  - nastavení titulku okna (panelu)
 *  - nastavení velikost okna (znaků na výšku a šířku) - testováno na:
 *    - (1) Windows 11 (24H2):
 *      - Windows Console Host (ConHost.exe) - via příkaz [mode]
 *        (moderní Windows Terminal není podporován)
 *        nastaví přímo optimální rozměry terminálu pro hru
 *    - (2) macOS 15.4:
 *      - Terminal - via AppleScript
 *        zajistí změnu velikost okna Terminálu v pixelech (x, y)
 *
 */


#ifndef TERM_SET_H
#define TERM_SET_H


#include <stdbool.h>


/* nastaví titulek okna (panelu) emulátoru terminálu
   (1) Windows - CMD, WT
   (2) UN*X - GNOME Terminal, KDE Konsole, macOS Terminal */
bool term_title(const char *novy_titulek);


/* nastaví rozměry okna (resp. buffer) emulátoru terminálu
   na určitý počet znaků - x: sloupečky, y: řádky
   (1) Windows - pro Console Host, Windows Terminal není podporován.
       Následující přepínače kompilátoru cl.exe zajistí otevření ConHost:
       [/link /subsystem:windows /entry:mainCRTStartup user32.lib].
       Více komentáře ve zdrojovém kódu.
   (2) macOS 15 - Terminal (pouze nastavení velikosti okna v pixelech)
   (3) Linux - NEIMPLEMENTOVÁNO (nabízí se např. využití D-Bus) */
bool term_size(int x, int y);


#endif
