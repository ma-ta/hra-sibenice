@echo off

::::::::::::::::::::::::::::::::::::::::
::                                    ::
::  SKRIPT NA SESTAVENI HRY SIBENICE  ::
::  (BATCH\DOS)                       ::
::                                    ::
::  autor:  Martin TABOR (Ma-TA)      ::
::  datum:  2022-07-18                ::
::                                    ::
::::::::::::::::::::::::::::::::::::::::


::::::::::::::::::::::::::::::::::::::::
:: KONFIGURACE:
::::::::::::::::::::::::::::::::::::::::


:: nazev spustitelneho souboru
   set bin_nazev=sibenice

:: parametry prekladace
   :: debug:    -Wall -Wextra -pedantic -g
   :: release:  
   set cc_param=-Wall -Wextra -pedantic -g

:: korenovy adresar se zdrojovymi kody
   set src_dir=.\source

:: korenovy adresar pro binarni soubory
   set out_dir=.\bin

:: oramovani
   set oramovani=----------------------------------


::::::::::::::::::::::::::::::::::::::::


cls

echo %oramovani%
echo SESTAVUJI - Vypis chyb a udalosti:
echo %oramovani%
echo.

:: zkopiruje slozku data do slozky bin
   mkdir %out_dir%\data
   xcopy %src_dir%\data %out_dir%\data /E /S

:: kompilace jednotlivych souboru

:: korenovy adresar
   cd %src_dir%
   gcc %cc_param% -c *.c
   cd ..
   move %src_dir%\*.o %out_dir%
:: slozka game
   cd %src_dir%\game
   gcc %cc_param% -c *.c
   cd ..\..
   move %src_dir%\game\*.o %out_dir%
:: slozka game\ukazatele
   cd %src_dir%\game\ukazatele
   gcc %cc_param% -c *.c
   cd ..\..\..
   move %src_dir%\game\ukazatele\*.o %out_dir%
:: slozka help
   cd %src_dir%\help
   gcc %cc_param% -c *.c
   cd ..\..
   move %src_dir%\help\*.o %out_dir%
:: slozka menu
   cd %src_dir%\menu
   gcc %cc_param% -c *.c
   cd ..\..
   move %src_dir%\menu\*.o %out_dir%
:: slozka tui
   cd %src_dir%\tui
   gcc %cc_param% -c *.c
   cd ..\..
   move %src_dir%\tui\*.o %out_dir%

:: sestaveni spustitelneho souboru
   cd %out_dir%
   gcc *.o -o %bin_nazev%.exe
   del *.o
   cd ..

echo.
echo %oramovani%
echo.

:: spusteni sestaveneho programu
   %out_dir%\%bin_nazev%.exe -v

:: vyčkání na stisk klávesy
   echo.
   echo %oramovani%
   echo.
   pause
