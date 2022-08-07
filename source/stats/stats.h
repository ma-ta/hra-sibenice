#ifndef STATS_H
#define STATS_H


/* pokusí se načíst soubor se statistikami
   a inicializuje modul
   true  = statistiky načteny,
   false = statistiky nenačteny */
bool stats_nastav(void);

/* zjistí, zda je modul
   statistik nastaven */
bool stats_nastaveno(void);

/* zjistí, zda byla správně
   načtena data ze souboru */
bool stats_nacteno(void);

/* informuje, zda jsou již
   uložena nějaká data */
bool stats_data(void);

/* pokusí se zapsat statistiky
   do datového souboru */
bool stats_uloz(void);

/* aktualizuje statistiky bodů
   dle aktuálních dat
   -1 = statistiky nejsou nastaveny
    0 = není zajímavé
    n = n-té nejvyšší skóre */
int stats_zpracuj_body(int body);

/* načte a uloží jméno hráče
   do struktury se statistikami */
void stats_zadej_jmeno(int pozice);

/* aktualizuje statistiky bodů
   dle aktuálních dat
   -1 = statistiky nejsou nastaveny
    0 = není zajímavé
    1 = patří mezi X nejvyšších
    2 = zatím zcela nejvyšší */
int stats_zpracuj_cas(struct tm *p_cas);

/* vrátí nejdelší herní čas
   prostřednictvím parametrů
   false = statistiky nejsou nacteny */
bool stats_zjisti_nejcas(int *h, int *min, int *s);

/* vrátí výši skóre pro zadanou pozici
   0 = nejsou k dispozici žádná data */
int stats_zjisti_nte_nejbody(int nte_poradi);

/* vrátí jméno hráče na n-té pozici,
   případně prázdný řetězec */
const char *stats_zjisti_nte_nejjmeno(int nte_poradi);

/* vypíše aktuální herní statistiky */
bool stats_vypis(bool jednoduchy_vypis);

/* smaže uložené statistiky */
bool stats_vymaz(void);

/* uvolní dynamicky alokovanou
   paměť v rámci modulu statistik */
void stats_vycisti(void);


#endif
