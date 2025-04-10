# Hra Š I _ E _ I C E

> Multiplatformní implementace známé slovní hry Šibenice pro terminál

![GitHub release (latest by date including pre-releases)](https://img.shields.io/github/v/release/ma-ta/hra-sibenice?include_prereleases)
![GitHub](https://img.shields.io/github/license/ma-ta/hra-sibenice)

![Šibenice – ikona](/res/github.png)

### Popis a stručné instrukce
Známá slovní hra (alias Oběšenec, Hangman) založená na hádání jednotlivých písmen tajného slova.
Program je napsán v jazyce C a pro jeho spuštění by neměly být potřeba žádné nestandardní knihovny.

- **[Základní instrukce k sestavení](how_make.txt)**
- [Nápověda ke hře](/res/napoveda.txt)
- [Známé chyby a nápady na další vývoj](/res/poznamky.txt)


### Binárky ke stažení

Rozbalte archiv ZIP a vyberte spustitelný soubor pro váš operační systém.

- **[All-in-one řešení (soubor ZIP)](bin/sibenice_v1.1.0.zip)**
- [Složka s binárkami](bin/)


#### Pozn. pro systémy typu MS-DOS
V případě zobrazení hlášky *"Load error: no DPMI - Get csdpmi\*.zip"* umístěte soubor [CWSDPMI.EXE](/bin/CWSDPMI.EXE) do stejného adresáře jako spustitelný soubor hry. Více o problematice např. [zde](//en.wikipedia.org/wiki/CWSDPMI) nebo [zde](https://sandmann.dotster.com/cwsdpmi/).

#### Pozn. pro systém macOS
Stažené binární soubory nejsou podepsané, proto je pravděpodobně bude možné spustit až po odebrání příslušného příznaku.<br>
Např. příkazem: [xattr -d com.apple.quarantine sibenice-macUni].

#### Použité kompilátory *(platformy)*:
- UN*X:
  - Ubuntu 24.04.2 LTS *(GNU GCC 13.3.0) &ndash; [x86] [x86_64] [AArch64]*
  - macOS 15.4 *(Apple Clang 17.0.0) &ndash; [x86_64] [AArch64] [Universal]*
- MS Windows:
  - Windows 11 *(MSVC 19.43.34810) &ndash; [x86] [x86_64] [AArch64]*
- DOS:
  - FreeDOS 1.4 *(DJGPP GCC 12.2.0) &ndash; [x86]*

### Snímky obrazovky

- [Všechny dostupné screenshoty zde](/res/screenshots)

![Šibenice&nbsp;&ndash;&nbsp;macOS 15.4 Terminal](/res/screenshots/hra-macos.png)

Hlavní menu:

![Šibenice&nbsp;&ndash;&nbsp;Menu](/res/screenshots/menu.png)

Obrazovka hry:

![Šibenice&nbsp;&ndash;&nbsp;Hra](/res/screenshots/hra.png)

Herní statistiky:

![Šibenice&nbsp;&ndash;&nbsp;Statistiky](/res/screenshots/kronika.png)

Obrazovka hry (DOS – 25x80 znaků):

![Šibenice&nbsp;&ndash;&nbsp;MS-DOS](res/screenshots/hra-dos.png)

Informace o verzi (DOS):

![Šibenice&nbsp;&ndash;&nbsp;MS-DOS](res/screenshots/prepinace-dos.png)
