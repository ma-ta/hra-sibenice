@echo off

rem :: SKRIPT PRO 32-BIT BINARKU:
rem :: (hra vyzaduje DOS extender - napr. CWSDPMI)

rem :: Testovane OS: FreeDOS 1.4

rem ::::::::::::::::::::::::::::::::::::::::
rem ::                                    ::
rem ::  SKRIPT NA SESTAVENI HRY SIBENICE  ::
rem ::  (FreeDOS / BATCH / DJGPP GCC)     ::
rem ::                                    ::
rem ::  autor:  Martin TABOR (Ma-TA)      ::
rem ::  datum:  2025-10-21                ::
rem ::                                    ::
rem ::::::::::::::::::::::::::::::::::::::::


rem ::::::::::::::::::::::::::::::::::::::::
rem :: KONFIGURACE:
rem ::::::::::::::::::::::::::::::::::::::::


rem :: nazev spustitelneho souboru
       set bin_nazev=sibe-d32

rem :: parametry pro prekladac (DJGPP)
       rem :: debug:    -Wall -Wextra -pedantic -g
       rem :: release:  -O3
           set cc_param=-O3

rem :: prikaz pro spusteni prekladace vc. parametru (gcc/wcc apod.)
       set cc=gcc

rem :: korenovy adresar se zdrojovymi kody
       set src_dir=.\source

rem :: korenovy adresar pro binarni soubory
       set out_dir=.\bin


rem :: oramovani (pouze pro vystup skriptu)
       set oramovani=----------------------------------


rem ::::::::::::::::::::::::::::::::::::::::


cls

echo %oramovani%
echo SESTAVUJI - Vypis chyb a udalosti:
echo %oramovani%
echo.

rem :: nastaveni prostredi pro WATCOMC
       set DJGPP=C:\DEVEL\DJGPP\DJGPP.ENV
       set PATH=C:\DEVEL\DJGPP\BIN;%PATH%

rem :: zkopiruje slozku data do slozky bin
       mkdir %out_dir%\data
       xcopy %src_dir%\data %out_dir%\data /E /S
rem :: zkopiruje info a napovedu do slozky bin
       copy %src_dir%\..\res\readme.md %out_dir%
       copy %src_dir%\..\res\napoveda.md %out_dir%

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
rem :: slozka libs
       cd %src_dir%\libs
       %cc% %cc_param% -c *.c
       cd ..\..
       move %src_dir%\libs\*.o %out_dir%
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
