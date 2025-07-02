/*
 *  (c) 2025  Martin TÁBOR
 *      Hra Šibenice v1.3.0
 *
 *  MODUL JE TŘEBA AKTIVOVAT DEFINOVÁNÍM SYMBOLICKÉ KONSTANTY:
 *  TERM_SET  1  (řešeno v globconf.h)
 *
 *  Modul pro nastavení emulátorů terminálu (multiplatformní řešení)
 *  - nastavení titulku okna (panelu)
 *  - nastavení velikost okna (znaků na výšku a šířku,
 *    testováno na:
 *    - (1) Windows 11 (24H2):
 *          - Windows Console Host (ConHost.exe) via příkaz [mode]
 *            (moderní Windows Terminal není podporován)
 *    - (2) macOS 15.4:
 *          - Terminal via AppleScript
 *  - nastavení typu fontu:
 *    pouze Windows Console Host (via WinAPI)
 *
 *  Ve Windows vyžaduje kompilaci přes cl.exe s přepínači
 *  [/link] - následující přepínače interpretuje linker:
 *
 *  - [/subsystem:windows]
 *    aplikace se bude spouštět jako GUI,
 *    což zamezí případnému otevření Windows Terminal,
 *
 *  - [/entry:mainCRTStartup]
 *    vstupním bodem aplikace zůstane main()
 *    a zároveň budou podporovány fce. z <stdio.h>,
 *
 *  - [user32.lib]
 *    pro volání některých fcí. WinAPI pro manipulaci s oknem
 *    (AllocConsole() totiž není okno ConHost aktivní a kód
 *    ho automaticky přesune do popředí, aby do něho šlo psát
 *    bez nutnosti kliknutí myší nebo použití Alt+Tab)
 */

#ifndef TERM_SET_H
#define TERM_SET_H


#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef enum {
   #if (defined(_WIN32) || defined(_WIN64) || defined(__DOS__) || defined(__MSDOS__) || defined(MSDOS))
      TERM_BLACK,
      TERM_BLUE,
      TERM_GREEN,
      TERM_AQUA,
      TERM_RED,
      TERM_PURPLE,
      TERM_YELLOW,
      TERM_WHITE,
      TERM_GRAY,
      TERM_LBLUE,
      TERM_LGREEN,
      TERM_LAQUA,
      TERM_LRED,
      TERM_LPURPLE,
      TERM_LYELLOW,
      TERM_LWHITE
   #else  /* ANSI sekvence */
      TERM_BLACK,
      TERM_RED,
      TERM_GREEN,
      TERM_YELLOW,
      TERM_BLUE,
      TERM_PURPLE,  /* magenta */
      TERM_AQUA,    /* cyan */
      TERM_WHITE,
      TERM_LRED,    /* níže světlé varianty převedené na "1;" */
      TERM_LGREEN,
      TERM_LYELLOW,
      TERM_LBLUE,
      TERM_LPURPLE,
      TERM_LAQUA,
      TERM_LWHITE
   #endif
} term_color;

typedef struct {
  term_color bg;
  term_color fg;
} term_color_bgfg;

extern int term_color_zap;  /* definováno v main.c */

/* nastaví barvu pozadí terminálu (resp. obrazovky),
   při zadání hodnoty 0 se daná barva nezmění,
   (1) DOS - volá příkaz [color]
   (2) Windows - pro Console Host pomocí WinAPI
   (3) macOS/UN*X - NEIMPLEMENTOVÁNO (otestovat ANSI sekvence) */
void term_barvy(term_color pozadi, term_color text);

/* nastavení původních barev terminálu */
void term_barvy_reset(void);

/* nastaví font terminálu
   (1) Windows - pro Console Host pomocí WinAPI */
void term_font(const char *font, int velikost, bool tucne);

/* nastaví titulek okna (panelu) emulátoru terminálu
   (1) Windows - CMD, WT
   (2) UN*X - GNOME Terminal, KDE Konsole, macOS Terminal */
bool term_title(const char *novy_titulek);

/* nastaví rozměry okna (resp. buffer) emulátoru terminálu
   na určitý počet znaků - x: sloupečky, y: řádky
   (1) Windows - pro Console Host, Windows Terminal není podporován.
       Následující přepínače kompilátoru cl.exe zajistí otevření ConHost:
       [/link /subsystem:windows /entry:mainCRTStartup user32.lib].
       Více komentáře ve zdrojovém kódu. Změna velikosti pomocí [mode]
   (2) macOS 15 - Terminal. Změna velikosti pomocí AppleScript
   (3) Linux - funguje pouze v GNOME Terminal (ne v kgx) a XTerm */
bool term_size(int x, int y);

/* v UN*X vrátí velikost terminálu do stavu před spuštěním programu */
void term_size_reset(void);

/* funkci je potřeba v některých OS zavolat při spuštění programu
   - ve Windows např. otevře terminál ConHost a přesměruje I/O
   - v UN*X uloží původní velikost terminálu před změnou jeho velikosti */
void term_init(void);

#endif  // TERM_SET_H
