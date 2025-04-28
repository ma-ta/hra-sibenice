```
        +-----------------------------------------------------+
        |                                                     |
        |         Ma-TA {}       H R A                        |
        |            games            S I _ E _ I C E         |
        |                                                     |
        +--------------------------+--------------------------+
        |    (c) 2025  Freeware    |          v1.2.0          |
        +--------------------------+--------------------------+

                             V i t e j t e


Aktualni  slozka  obsahuje  spustitelne  soubory hry. Podslozka DATA je
nezbytna pro spravny beh aplikace a pro ukladani hernich statistik.

Ke  spusteni  tedy staci "poklikat" na binarni soubor odpovidajici vasi
platforme (ve Windows) nebo dany soubor primo spustit v okne terminalu.
Minimalni  rozmery  terminalu  jsou  73 zn. na sirku a 36 zn. na vysku,
v DOSu nebo pri spusteni s prepinacem [-c] vystaci 73 x 25 znaku.

V  soucasne  dobe hra nepodporuje ceskou diakritiku a hadana slova jsou
tedy zobrazena bez hacku a carek!


Obsah aktualni slozky (nemusi byt kompletni):

./
 info.md              - (tento) soubor s rychlymi instrukcemi  (textovy)
 napoveda.md          - kompletni napoveda ke hre              (textovy)
 sibenice-linux32     - platforma GNU/Linux (32-bit, x86)      (program)
 sibenice-linux64     - platforma GNU/Linux (64-bit, x86-64)   (program)
 sibenice-linuxArm64  - platforma GNU/Linux (64-bit, AArch64)  (program)
 sibenice-macUni      - platforma macOS     (Intel & Silicon)  (program)
 sibenice-mac64       - platforma macOS     (64-bit, x86_64)   (program)
 sibenice-macArm64    - platforma macOS     (64-bit, AArch64)  (program)
 sibenice-win32.exe   - platforma Windows   (32-bit, x86)      (program)
 sibenice-win64.exe   - platforma Windows   (64-bit, x86-64)   (program)
 sibenice-winArm64    - platforma Windows   (64-bit, AArch64)  (program)
 sibe-d16.exe         - platforma DOS       (16-bit, x86)      (program)
 sibe-d32.exe         - platforma DOS       (32-bit, x86)      (program)
 CWSDPMI.EXE          - nutne pro 32-bit verzi v MS-DOS        (binarni)

data/
 (stats.sav)          - soubor s ulozenymi statistikami        (binarni)
                        (vytvoren az po prvnim spusteni hry)
 game_cs.dat          - slovnik 1000 nahodnych ceskych slov    (textovy)
 game.dat             - zdrojovy soubor ascii-artu sibenice    (textovy)
 help.dat             - zdrojovy soubor napovedy pro aplikaci  (textovy)
 stats.dat            - zdrojovy soubor obrazovky statistik    (textovy)

wasm/                 - platforma WebAssembly
 index.html           - hlavni stranka (vyzaduje http server)  (webpage)
 (*)                  - dalsi soubory potrebne pro index.html

------------------------------------------------------------------------

Autor         :  (c) 2022-25  Martin TABOR
Licence       :  GNU GPLv3+ (Freeware & Open source)
Zdrojove kody :  https://github.com/ma-ta/hra-sibenice
```
