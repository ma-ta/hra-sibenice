```
------------
ZNÁMÉ CHYBY:
------------

1) není zajištěno, aby se v rámci kola/hry nemohla opakovat slova
   (leč pseudonáhodná čísla jsou generována v rovnoměrném rozložení)

2) není zajištěno, aby se v rámci kola nemohly ihned za sebou
   opakovat stejné hlášky o výsledku tahu
   (tzn. vyloučit poslední položku nebo změnit na carousel)

3) při zadávání jména do kroniky může způsobit problémy, když uživatel
   použije jiné znaky než ASCII (chybné zarovnání na obr. statistik)


Hotovo:

3) [v1.2.1]
   lépe vyřešeno načítání slov ze souboru slovníku v modulu [hra.c]:
   - nenačtou se prázdné řádky či řádky se znaky mezery (' ')
   - paměť dynamicky alokována dle skutečného počtu načtených slov ze souboru
     (počet slov v souboru je variabilní)
   - zavedeno makro HRA_POCETSLOV_MIN v [globconf.h] umožňující stanovit
     minimální velikost slovníku
   - celkově robustnější řešení a ošetření chyb

2) [v0.0.4]
   hledání písmen 'CH' při zadání písmene 'C'
   (při hledání 'H' jsou naopak přeskočeny výskyty zn. v rámci 'CH')

1) [v0.0.4]
   nelze načíst externí soubory, pakliže je program spuštěn z
   jiného adresáře než se nachází binárka s hrou
   => vyřešeno funkcí prepni_adresar() v main.c


----------------------
NÁPADY NA DALŠÍ VÝVOJ:
----------------------

1) vylepšit odkrývání spřežek/skupin znaků (české písmeno 'CH' apod.)
   (definice znaků budou např. součástí slovníků pro jednotlivé jazyky)

2) nová zvláštní volba při probíhající hře
   [2] - doplní chybějící písmeno za cenu určitého počtu bodů
         dle aktuální situace:
         a) nejpočetněji zastoupené chybějící písmeno (první zleva)
         b) první chybějící písmeno zleva

3) možnost volby různých kategorií hádaných slov / vícejazyčná verze

4) začlenění knihovny getch - program bude reagovat přímo na stisk kláves
   (písmen) bez nutnosti potvrzování přes Enter

5) možnost hrát ve dvou (příp. i více) lidech
   - bude se přepínat mezi jednotlivými účastníky
   - zvítězí ten, kdo vydrží nejdéle
   - v případě remízy při dosažení max. počtu kol hráč s nejvyšším skóre

6) přidání diakritiky

7) distribuce hry v linuxových repozitářích, MS Store, FreeDOS apod.

8) balíčky pro distribuci aplikace:
   GNU/Linux - AppImage,
   macOS     - formáty DMG a APP

9) v modulu term_set přidat podporu pro změnu velikosti písma emu. terminálu
   (výchozí písmo po instalaci OS může být pro hru příliš malé
    + volitelně pojistit monospaced font)

10) zašifrovat seznam hádaných slov ve slovníku [data/game_cs.dat]
    (např. Caesarova šifra, Vernamova šifra či cokoliv "zábavného")


Hotovo:

3) [v1.1.0]
   instalátor pro Windows (via Inno Setup)

2) [v0.0.3]
   přidání "Síně slávy / Herní kroniky" do hlavního menu - výsledky úspěšně dokončených
   her se budou ukládat spolu se jménem hráče / zobrazení statistik,
   výsledku poslední hry apod. na obrazovce s hlavním menu

1) [v0.0.2]
   možnost ukončit hru v průběhu hádání slov (příp. zobrazit nápovědu
   a vrátit se zpět do hry)
```
