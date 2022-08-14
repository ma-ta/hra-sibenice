@echo off

::::::::::::::::::::::::::::::::::::::::
::                                    ::
::  SKRIPT NA SESTAVENI HRY SIBENICE  ::
::  (BATCH\WINDOWS)                   ::
::                                    ::
::  autor:  Martin TABOR (Ma-TA)      ::
::  datum:  2022-07-18                ::
::                                    ::
::::::::::::::::::::::::::::::::::::::::


::::::::::::::::::::::::::::::::::::::::
:: KONFIGURACE:
::::::::::::::::::::::::::::::::::::::::


:: nazev spustitelneho souboru
   set bin_nazev=sibenice_win

:: parametry prekladace
   :: debug:    /Wall /D_CRT_SECURE_NO_WARNINGS
   :: release:  /D_CRT_SECURE_NO_WARNINGS
   set cc_param=/Wall /D_CRT_SECURE_NO_WARNINGS

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
   cl %cc_param% /c *.c
   cd ..
   move %src_dir%\*.obj %out_dir%
:: slozka game
   cd %src_dir%\game
   cl %cc_param% /c *.c
   cd ..\..
   move %src_dir%\game\*.obj %out_dir%
:: slozka game\game_tui
   cd %src_dir%\game\game_tui
   cl %cc_param% /c *.c
   cd ..\..\..
   move %src_dir%\game\game_tui\*.obj %out_dir%
:: slozka help
   cd %src_dir%\help
   cl %cc_param% /c *.c
   cd ..\..
   move %src_dir%\help\*.obj %out_dir%
:: slozka menu
   cd %src_dir%\menu
   cl %cc_param% /c *.c
   cd ..\..
   move %src_dir%\menu\*.obj %out_dir%
:: složka stats
   cd %src_dir%\stats
   cl %cc_param% /c *.c
   cd ..\..
   move %src_dir%\stats\*.obj %out_dir%
:: slozka tui
   cd %src_dir%\tui
   cl %cc_param% /c *.c
   cd ..\..
   move %src_dir%\tui\*.obj %out_dir%

:: sestaveni spustitelneho souboru
   cd %out_dir%
   cl /Fe%bin_nazev%.exe *.obj
   del *.obj
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
