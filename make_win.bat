@echo off

:: Nutno spustit v prostredi Developer Command Prompt (for MS Visual Studio)

::::::::::::::::::::::::::::::::::::::::
::                                    ::
::  SKRIPT NA SESTAVENI HRY SIBENICE  ::
::  (WINDOWS / BATCH / MSVC)          ::
::                                    ::
::  autor:  Martin TABOR (Ma-TA)      ::
::  datum:  2025-04-01                ::
::                                    ::
::::::::::::::::::::::::::::::::::::::::


::::::::::::::::::::::::::::::::::::::::
:: KONFIGURACE:
::::::::::::::::::::::::::::::::::::::::


:: nazev spustitelneho souboru
   :: -win64 / -winArm64 / -win32 / -...
   set bin_nazev=sibenice-win

:: parametry prekladace
   :: debug:    /D_CRT_SECURE_NO_WARNINGS /W4 /Od /Zi /DEBUG /fsanitize=address
   :: release:  /D_CRT_SECURE_NO_WARNINGS /W2 /O2 /GL
   set cc_param=/D_CRT_SECURE_NO_WARNINGS /W2 /O2 /GL

:: prikaz pro spusteni prekladace vc. parametru (cl/clang apod.)
   set cc=cl.exe

:: korenovy adresar se zdrojovymi kody
   set src_dir=.\source

:: korenovy adresar pro binarni soubory
   set out_dir=.\bin

:: resource soubory
   set res_dir=.\res
   set vs_version_info=win-versioninfo

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

:: resource files
   rc.exe %res_dir%\%vs_version_info%.rc
   move %res_dir%\%vs_version_info%.res .\%out_dir%
:: korenovy adresar
   cd %src_dir%
   %cc% %cc_param% /c *.c
   cd ..
   move %src_dir%\*.obj %out_dir%
:: slozka game
   cd %src_dir%\game
   %cc% %cc_param% /c *.c
   cd ..\..
   move %src_dir%\game\*.obj %out_dir%
:: slozka game\game_tui
   cd %src_dir%\game\game_tui
   %cc% %cc_param% /c *.c
   cd ..\..\..
   move %src_dir%\game\game_tui\*.obj %out_dir%
:: slozka help
   cd %src_dir%\help
   %cc% %cc_param% /c *.c
   cd ..\..
   move %src_dir%\help\*.obj %out_dir%
:: slozka menu
   cd %src_dir%\menu
   %cc% %cc_param% /c *.c
   cd ..\..
   move %src_dir%\menu\*.obj %out_dir%
:: složka stats
   cd %src_dir%\stats
   %cc% %cc_param% /c *.c
   cd ..\..
   move %src_dir%\stats\*.obj %out_dir%
:: slozka tui
   cd %src_dir%\tui
   %cc% %cc_param% /c *.c
   cd ..\..
   move %src_dir%\tui\*.obj %out_dir%

:: sestaveni spustitelneho souboru
   cd %out_dir%
   %cc% %cc_param% /Fe%bin_nazev%.exe *.obj %vs_version_info%.res
   del *.obj *.res
   cd ..


:: spusteni sestaveneho programu
   echo.
   echo %oramovani%
   echo Spoustim:
   echo %out_dir%\%bin_nazev%.exe
   echo %oramovani%
   echo.

   %out_dir%\%bin_nazev%.exe -v

:: vyckani na stisk klavesy
   echo.
   echo %oramovani%
   echo.
   pause
