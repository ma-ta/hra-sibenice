/*
 *  (c) 2025 - Hra Šibenice v1.1.0  |  Martin TÁBOR
 *
 *  Modul pro nastavení emulátorů terminálu (multiplatformní řešení)
 *  - nastavení titulek okna (panelu)
 *  - nastavení velikost okna (znaků na výšku a šířku) - testováno na:
 *    - Windows:
 *      - Command Line (Windows Console Host) - via příkaz [mode]
 *        (moderní Windows Terminal není podporován)
 *    - Ubuntu/Kubuntu:
 *      - GNOME Terminal/Console, KDE Konsole - via D-BUS
 *    - macOS:
 *      - Terminal - via AppleScript
 *
 */


#ifndef TERM_SET_H
#define TERM_SET_H


/* nastaví titulek okna (panelu) emulátoru terminálu */
bool term_title(void);
/* nastaví rozměry okna (buffer) emulátoru terminálu
   na určitý počet znaků - x: sloupečky, y: řádky */
bool term_size(int x, int y);


#endif
