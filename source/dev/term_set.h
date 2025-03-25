/*
 *  (c) 2025 - Hra Šibenice v1.1.0
 *
 *  Modul pro nastavení emulátorů terminálu (multiplatformní řešení)
 *  - nastavení titulku okna (panelu)
 *  - nastavení velikost okna (znaků na výšku a šířku) - testováno na:
 *    - (1) Windows:
 *      - Command Line (Windows Console Host) - via příkaz [mode]
 *        (moderní Windows Terminal není podporován)
 *        nastaví přímo optimální rozměry terminálu pro hru
 *    - (2) macOS 15:
 *      - Terminal - via AppleScript
 *        zajistí připnutí okna k pravému okraji obrazovky
 *
 */


#ifndef TERM_SET_H
#define TERM_SET_H


/* nastaví titulek okna (panelu) emulátoru terminálu
   (1) Windows - CMD, WT
   (2) UN*X - GNOME Terminal, KDE Konsole, macOS Terminal */
bool term_title(void);


/* nastaví rozměry okna (resp. buffer) emulátoru terminálu
   na určitý počet znaků - x: sloupečky, y: řádky
   (1) Windows - CMD, WT není podporován
   (2) macOS 15 - Terminal (pouze připnutí k pravému okraji monitoru)
   (3) Linux - nabízí se D-Bus API různých terminálů
               (prozatím není implementováno) */
bool term_size(int x, int y);


#endif
