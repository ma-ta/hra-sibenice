```
+-------------+
| ZNÁMÉ CHYBY |
+-------------+


5) [v1.3.0]
   - v UN*Xu je po spuštění hry zobrazeno běžné písmo a v jejím
     průběhu nebo po ukončení se najednou font změní na tučný
   - nejsou vyřešeny možné kombinace přepínačů (-wc, -cw, ...)
   - při velmi pomalém vykreslování je zřejmé, že se nejdříve změní
     podbarvení terminálu (při aktuálně vykresleném textu), poté se
     text vymaže a vykreslí nový (alespoň při použití příkazu color
     v MS-DOS
   - při pomalém HW (např. v DOSBox-X) pozorováno jiné chování při
     změně pozadí na zelenou během hry (u jiných barev ne)
   - ne zcela správně implementovaný převod atributů příkazové řádky
     ve fci. main() na malá písmena (lépe změnit fce. porovnávání)

4) [v1.2.1]
   poslední slovo ve slovníku musí být ukončeno novým řádkem ('\n'),
   jinak je vypsána chyba, že se nevešlo do bufferu (není načteno)

3) při zadávání jména do kroniky může způsobit problémy, když uživatel
   použije jiné znaky než ASCII (chybné zarovnání na obr. statistik)
2) není zajištěno, aby se v rámci kola nemohly ihned za sebou
   opakovat stejné hlášky o výsledku tahu
   (tzn. vyloučit poslední položku nebo změnit na carousel)

1) není zajištěno, aby se v rámci kola/hry nemohla opakovat slova
   (leč pseudonáhodná čísla jsou generována v rovnoměrném rozložení)


Hotovo:

4) [v1.3.0]
   - přepínače nově nejsou case sensitive (vhodné pro DOS a WIN)
   - zobrazí se hláška před načítáním slov ze slovníku
     (obrazovka tedy na pomalém HW zdánlivě pouze nezamrzne)

3) [v1.2.1]
   lépe vyřešeno načítání slov ze souboru slovníku v modulu [hra.c]:
   - nenačtou se prázdné řádky či řádky se znaky mezery (' '),
   - paměť dynamicky alokována dle skutečného počtu načtených slov ze souboru
     (počet slov v souboru je variabilní),
   - zavedeno makro HRA_POCETSLOV_MIN v [globconf.h] umožňující stanovit
     minimální velikost slovníku,
   - případná duplicitní slova ve slovníku se načtou a započítají do limitu
     HRA_POCETSLOV_MIN jen jednou,
   - celkově robustnější řešení a ošetření chyb.

2) [v0.0.4]
   hledání písmen 'CH' při zadání písmene 'C'
   (při hledání 'H' jsou naopak přeskočeny výskyty zn. v rámci 'CH')

1) [v0.0.4]
   nelze načíst externí soubory, pakliže je program spuštěn z
   jiného adresáře než se nachází binárka s hrou
   => vyřešeno funkcí prepni_adresar() v main.c


+-----------------------+
| NÁPADY NA DALŠÍ VÝVOJ |
+-----------------------+


------------------------------------------------------------------------------

+--------+------------+
| v1.4.0 | ????-??-?? |  -  V PLÁNU
+--------+------------+


NOVINKY:

1) vylepšení slovníku (hádaných slov):
   a) možnost vlastního slovníku pomocí přepínače [-v] [cesta_k_souboru]:
      - umožní načíst slova z vlastního slovníku,
      - vypne dešifrování při načítání slov,
      - vypne hlídání minimálního počtu slov ve slovníku,
      - vypne ukládání herních statistik (kromě odehráté doby).

   b) první znak '#' na řádku slovníku značí komentář, který bude při
      načítání souboru přeskočen (to umožní vepsat libovolné informace)

2) vylepšení vzhledu TUI - žluté pozadí:
   - při dosažení předposledního kola hry (8. a 9. hádané slovo),
   - při zobrazení HERNÍ KRONIKY z hlavního menu.

------------------------------------------------------------------------------


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
```
