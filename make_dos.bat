@echo off

rem Testovane OS: FreeDOS 1.3 az 1.4

rem ::::::::::::::::::::::::::::::::::::::::
rem ::                                    ::
rem ::  SKRIPT NA SESTAVENI HRY SIBENICE  ::
rem ::  (DOS / BATCH / DJGPP)             ::
rem ::                                    ::
rem ::  autor:  Martin TABOR (Ma-TA)      ::
rem ::  datum:  2025-04-10                ::
rem ::                                    ::
rem ::::::::::::::::::::::::::::::::::::::::


rem ::::::::::::::::::::::::::::::::::::::::
rem :: KONFIGURACE:
rem ::::::::::::::::::::::::::::::::::::::::


rem :: nazev spustitelneho souboru
       set bin_nazev=sibe-dos

rem :: parametry pro prekladac (DJGPP)
       rem :: debug:    -Wall -Wextra -pedantic -g
       rem :: release:  -O2
           set cc_param=-O2

rem :: prikaz pro spusteni prekladace vc. parametru (gcc/wcc apod.)
       set cc=gcc

rem :: korenovy adresar se zdrojovymi kody
       set src_dir=.\source

rem :: korenovy adresar pro binarni soubory
       set out_dir=.\bin

rem :: oramovani
       set oramovani=----------------------------------


rem ::::::::::::::::::::::::::::::::::::::::


cls

echo %oramovani%
echo SESTAVUJI - Vypis chyb a udalosti:
echo %oramovani%
echo.

rem :: zkopiruje slozku data do slozky bin
       mkdir %out_dir%\data
       xcopy %src_dir%\data %out_dir%\data /E /S

rem :: kompilace jednotlivych souboru

rem :: korenovy adresar
       cd %src_dir%
       %cc% %cc_param% -c *.c
       cd ..
       move %src_dir%\*.o %out_dir%
rem :: slozka game
       cd %src_dir%\game
       %cc% %cc_param% -c *.c
       cd ..\..
       move %src_dir%\game\*.o %out_dir%
rem :: slozka game\game_tui
       cd %src_dir%\game\game_tui
       %cc% %cc_param% -c *.c
       cd ..\..\..
       move %src_dir%\game\game_tui\*.o %out_dir%
rem :: slozka help
       cd %src_dir%\help
       %cc% %cc_param% -c *.c
       cd ..\..
       move %src_dir%\help\*.o %out_dir%
rem :: slozka menu
       cd %src_dir%\menu
       %cc% %cc_param% -c *.c
       cd ..\..
       move %src_dir%\menu\*.o %out_dir%
rem :: složka stats
       cd %src_dir%\stats
       %cc% %cc_param% -c *.c
       cd ..\..
       move %src_dir%\stats\*.o %out_dir%
rem :: slozka tui
       cd %src_dir%\tui
       %cc% %cc_param% -c *.c
       cd ..\..
       move %src_dir%\tui\*.o %out_dir%

rem :: sestaveni spustitelneho souboru
       cd %out_dir%
       %cc% %cc_param% -o %bin_nazev%.exe *.o
       del *.o
       cd ..


rem :: spusteni sestaveneho programu
       echo.
       echo %oramovani%
       echo Spoustim:
       echo %out_dir%\%bin_nazev%.exe
       echo %oramovani%
       echo.

       %out_dir%\%bin_nazev%.exe -v

rem :: vyckani na stisk klavesy
       echo.
       echo %oramovani%
       echo.
       pause
