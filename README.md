# Hra Š I _ E _ I C E

> Terminálová implementace známé slovní hry Šibenice/Oběšenec

![GitHub release (latest by date including pre-releases)](https://img.shields.io/github/v/release/ma-ta/hra-sibenice?include_prereleases)
![GitHub](https://img.shields.io/github/license/ma-ta/hra-sibenice)

### Popis a stručné instrukce
Známá slovní hra (alias Oběšenec, Hangman) založená na hádání jednotlivých písmen tajného slova.
Program je napsán v jazyce ANSI C a pro jeho spuštění by neměly být potřeba žádné nestandardní knihovny.

- **[Nápověda ke hře](/res/napoveda.txt)**
- [Známé chyby a nápady na další vývoj](/res/poznamky.txt)


### Binárky ke stažení

Rozbalte archiv ZIP a vyberte spustitelný soubor pro váš operační systém.

- **[All-in-one řešení (soubor ZIP)](//github.com/ma-ta/hra-sibenice/releases/download/v0.0.3/sibenice_0.0.3_bin.zip)**
- [Složka s binárkami](bin/release/)


#### Pro systém MS-DOS (DOSBox)
V případě zobrazení hlášky *"Load error: no DPMI - Get csdpmi\*.zip"* umístěte soubor [CWSDPMI.EXE](/bin/release/sibenice_0.0.3_dos/cwsdpmi.exe) do stejného adresáře jako spustitelný soubor hry. Více o problematice např. [zde](//en.wikipedia.org/wiki/CWSDPMI) nebo [zde](https://sandmann.dotster.com/cwsdpmi/).


#### Zahrnuté (testované) platformy:
- MS Windows *(překladač MSVC)*:
  - Windows 11
- GNU/Linux *(překladač GNU GCC)*:
  - Ubuntu 22.04 (LTS)
- DOS *(překladač DJGPP)*:
  - FreeDOS 1.3
  - MS-DOS 6.22


### Snímky obrazovky

- [Všechny dostupné screenshoty zde](/res/screenshots)

Obrazovka hry:

![Šibenice 0.0.3](/res/screenshots/hra.png)

Obrazovka hry - DOS mód:

![Šibenice 0.0.3 DOS](res/screenshots/hra-dos.png)