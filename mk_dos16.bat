@echo off

rem :: SKRIPT PRO 16-BIT BINARKU:

rem :: Testovane OS: FreeDOS 1.4

rem ::::::::::::::::::::::::::::::::::::::::
rem ::                                    ::
rem ::  SKRIPT NA SESTAVENI HRY SIBENICE  ::
rem ::  (FreeDOS / BATCH / OPEN WATCOM)   ::
rem ::                                    ::
rem ::  autor:  Martin TABOR (Ma-TA)      ::
rem ::  datum:  2025-10-30                ::
rem ::                                    ::
rem ::::::::::::::::::::::::::::::::::::::::


rem ::::::::::::::::::::::::::::::::::::::::
rem :: KONFIGURACE:
rem ::::::::::::::::::::::::::::::::::::::::


rem :: nazev spustitelneho souboru
       set bin_nazev=sibe-dos

rem :: verze Open Watcom (zjistitelne prikazem [wcc])
       rem ::set cc_ver=1.9  :: (makro __WATCOMC__ == např. 1900)

rem :: nastaveni velikosti zasobniku pro [wcl] (8 KB == Stack Overflow!)
       rem :: (hra vyzaduje vetsi zasobnik nez je vychozi u Open Watcom)
       set wcl_stack=16384

rem :: parametry pro prekladac (Open Watcom)
       rem :: debug:    -q -g -k%wcl_stack%   ::-dWATCOMC_VER=%cc_ver%
       rem :: release:  -q -ox -k%wcl_stack%  ::-dWATCOMC_VER=%cc_ver%
           set cc_param=-q -ox -k%wcl_stack%

rem :: prikaz pro spusteni prekladace vc. parametru (gcc/wcl,wcc apod.)
       set cc=wcl

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
       call c:\devel\watcomc\owsetenv.bat

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
       move %src_dir%\*.obj %out_dir%
rem :: slozka game
       cd %src_dir%\game
       %cc% %cc_param% -c *.c
       cd ..\..
       move %src_dir%\game\*.obj %out_dir%
rem :: slozka game\game_tui
       cd %src_dir%\game\game_tui
       %cc% %cc_param% -c *.c
       cd ..\..\..
       move %src_dir%\game\game_tui\*.obj %out_dir%
rem :: slozka help
       cd %src_dir%\help
       %cc% %cc_param% -c *.c
       cd ..\..
       move %src_dir%\help\*.obj %out_dir%
rem :: slozka libs
       cd %src_dir%\libs
       %cc% %cc_param% -c *.c
       cd ..\..
       move %src_dir%\libs\*.obj %out_dir%
rem :: slozka menu
       cd %src_dir%\menu
       %cc% %cc_param% -c *.c
       cd ..\..
       move %src_dir%\menu\*.obj %out_dir%
rem :: složka stats
       cd %src_dir%\stats
       %cc% %cc_param% -c *.c
       cd ..\..
       move %src_dir%\stats\*.obj %out_dir%
rem :: slozka tui
       cd %src_dir%\tui
       %cc% %cc_param% -c *.c
       cd ..\..
       move %src_dir%\tui\*.obj %out_dir%

rem :: sestaveni spustitelneho souboru (slinkovani: wcl, wlink)
       cd %out_dir%
       %cc% %cc_param% -fe=%bin_nazev%.exe *.obj
       del *.obj
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
