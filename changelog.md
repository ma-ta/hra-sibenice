```
+--------+------------+
| v1.3.0 | 2025-06-09 |
+--------+------------+


NOVINKY:

1) možnost vlastního slovníku pomocí přepínače [-v] [cesta_k_souboru]:
   - umožní načíst slova z vlastního slovníku
   - vypne dešifrování při načítání slov
   - vypne hlídání minimálního počtu slov ve slovníku
   - vypne ukládání herních statistik (kromě odehráté doby)

2) první znak '#' na řádku slovníku značí komentář, který bude při
   načítání souboru přeskočen (to umožní vepsat libovolné informace)

3) vylepšení vzhledu herního TUI:
   - obarvení pozadí a písma terminálu dle aktuálního stavu hry
   - Windows: automatické nastavení fontu a jeho velikosti


OPRAVENÉ CHYBY:

1) všechny přepínače lze nově zadat buď malým nebo velkým písmenem

2) při načítání slov ze slovníku se zobrazí upozornění
   (vhodné u pomalých systémů s MS-DOS apod.)


+--------+------------+
| v1.2.1 | 2025-05-16 |
+--------+------------+


NOVINKY:

1) nové binárky:
   - Android (např. pro emulátor terminálu Termux)  [AArch64]


OPRAVENÉ CHYBY:

1) vylepšeno načítání slov ze slovníku v kódu [source/game/hra.c]:
   - automatické zjištění počtu slov v souboru,
   - definován minimální počet slov, který je požadován
     v [source/globconf.h] přes [HRA_POCETSLOV_MIN].

2) obsah slovníku [source/data/game_cs.dat] nebude nadále zašifrovaný
   (pouze jeho kopie ve složce [bin/data])

3) opravy v seznamu hádaných slov - slovník [game_cs.dat]:
   - nahrazeno několik nevhodných slov,
   - nahrazena jedna duplicita,
   - slovník v [source/data] seřazen dle abecedy, přičemž
     řádky šifrované kopie v [bin/data] odpovídají předloze.

4) při spuštění s přepínačem [-s] se již nemaže obrazovka
   (vhodnější např. pro možnost přesměrování výstupu do souboru)


+--------+------------+
| v1.2.0 | 2025-05-05 |
+--------+------------+


NOVINKY:

1) webová verze hry na bázi WebAssembly a Xterm.js
   (lze spustit lokálně téměř v jakémkoli v prohlížeči)

2) zašifrování hádaných slov v datovém souboru [game_cs.dat]
   => slovník již nelze jednoduše přečíst nebo podvrhnout
   vlastním "alternativním" seznamem slov (v tomto případě
   se ve hře jednoduše objeví nesmyslné řetězce)

3) nové binárky:
   - FreeBSD [x86-64] [AArch64]
   - Linux   [armhf] [riscv64]


OPRAVENÉ CHYBY:

1) barevné texty přepnuty na jasnější varianty pro
   lepší čitelnost (ANSI escape code "1;")


+--------+------------+
| v1.1.0 | 2025-04-12 |
+--------+------------+


NOVINKY:

1) výpis typu architektury v informacích o verzi [-v],
   rozšírení dostupných binárek o nové architektury:
   - Windows [x86-64] [AArch64] [x86]
   - Linux   [x86-64] [AArch64] [x86]
   - macOS   [AArch64] [x86-64] [Universal]
   - DOS     [x86 16-bit] [x86 32-bit]

2) ikona a metadata aplikace:
   - Windows: pro kompilátor MSVC ikona a soubor
     VERSIONINFO resource (win-versioninfo.rc)
   - macOS: vytvořen Figma template a ikona .icns
     ve složce [res/icons/mac]

3) název aplikace jako titulek okna (panelu) terminálu,
   automatické nastavení velikosti okna terminálu
   pomocí přepínače [w]:
   a) nastavení konkrétní velikost terminálu po spuštění
      "-w [x_znaků y_znaků]"
   b) vypnutí automatického nastavení velikosti okna
      "-w"


OPRAVENÉ CHYBY:

1) Ubuntu 24.04/GCC - redukce zbytečných chybových hlášek při kompilaci
   (v make.sh přidán parametr: -Wno-unused-result)

2) macOS - korektní přepnutí pracovního adresáře při
   spuštění přes Finder

3) DOS - již se nevypisují zbytečné přepínače (-c, -w)


+--------+------------+
| v1.0.0 | 2024-02-29 |
+--------+------------+


NOVINKY:

1) drobné úpravy v textech napříč obrazovkami

2) obarvení některých prvků pomocí ANSI escape kódů
   (herní obrazovka + statistiky)

3) vytvořena ikona a banner aplikace - ve složce [res]


OPRAVENÉ CHYBY:

1) v cs slovníku nahrazeno nevhodné slovo hezkým
   českým slovem "klapkobřinkostroj"

2) po ukončení hry se nemaže terminál

3) změna přípony souboru se statistikami ve složce [data]
   (.bin -> .sav)


+--------+------------+
| v0.0.4 | 2022-10-02 |
+--------+------------+


NOVINKY:

1) zprovozněno odkrývání písmen 'CH' při zadání znaku 'C'
   (při hledání písmen 'H' jsou naopak přeskočeny výskyty
    znaku 'H' v rámci všech písmen 'CH' ve slově)


OPRAVENÉ CHYBY:

1) otevírání souborů pomocí relativních cest
   při spuštění hry z jiného adresáře

2) před ukončením hry zvláštní volbou (vyhrazeným zn.)
   se zobrazí žádost o potvrzení (a/n)

3) stisknutí Enteru před ukončením aplikace
   (zabrání samovolnému zavření okna příkazové řádky)

4) odebrán zvonek ('\a') při každém zobrazení šibenice
   v režimu DOS (příliš rušivý prvek)


+--------+------------+
| v0.0.3 | 2022-08-15 |
+--------+------------+


OPRAVENÉ CHYBY:

1) kompilace pomocí DJGPP pod FreeDOS
   (zkráceny názvy souborů na max. 8 znaků)

2) vykreslování ukazatele kol
   (opravena chyba s chybějícím pravým orámováním)


NOVINKY:

1) přepínače pro spuštění programu
   [?] - zobrazí seznam dostupných přepínačů
   [m] - zobrazí herní manuál
   [s] - vypíše uložené statistiky
   [v] - vypíše informace o verzi
   [c] - spustí v režimu CLI 80x25 zn. (DOS)

2) vylepšení modulu tui/hlavicka
   (možnost vypsání libovolného textu v dolních buňkách)

3) herní statistiky
   - nová položka v hlavním menu
   - upravená obrazovka s výsledkem hry
   - přepínač [s] vypíše herní statistiky

4) informace o době běhu programu po ukončení


+--------+------------+
| v0.0.2 | 2022-07-18 |
+--------+------------+


OPRAVENÉ CHYBY:

1) při vypnutí ansi_format v konfiguraci chyba kompilace
   (makro musí vracet prázdný řetězec "")

2) po vstupu do nápovědy se opětovně nezaplo zobrazení
   kurzoru v případě chyby otevření souboru s nápovědou

3) opraven znak UKAZATELPISMEN_NIC v modulu ukazatel_pismen
   (původní '0' umožňoval hádat nulu jako dostupný znak)

4) při zadání chybné volby v hlavním menu se zobrazí hláška


NOVINKY:

1) vylepšení kompatibility se systémy typu DOS (viz poznamky-dos.txt)
   - vypnutí formátování konzolového výstupu pod DOSem
   - úprava rozhraní pro rozlišení 25x80 znaků:
     a) vykreslování herního skóre do tabulky a zobrazení oběšence na
        samostatné obrazovce
     b) úprava rozložení nápovědy na 4 obrazovky a drobné změny v textu
        (vč. opravy vkládání nadbytečného prázdného řádku za zarážky '#'
        umístěné v souboru napoveda.dat)

2) vylepšená grafická podoba hlavičky a dalších obrázků typu ASCII-art

3) přidání zvláštních voleb do probíhající hry (vyhrazených znaků)
   [*] - zobrazí seznam dostupných voleb
   [0] - kdykoli ukončí probíhající hru
   [1] - zobrazí velkou nápovědou
```
