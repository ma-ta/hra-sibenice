@echo off

:: Nutno spustit v prostredi Developer Command Prompt (for MS Visual Studio)

::::::::::::::::::::::::::::::::::::::::
::                                    ::
::  SKRIPT NA SESTAVENI HRY SIBENICE  ::
::  (WINDOWS / BATCH / MSVC)          ::
::                                    ::
::  autor:  Martin TABOR (Ma-TA)      ::
::  datum:  2025-10-23                ::
::                                    ::
::::::::::::::::::::::::::::::::::::::::


::::::::::::::::::::::::::::::::::::::::
:: KONFIGURACE:
::::::::::::::::::::::::::::::::::::::::


:: nazev spustitelneho souboru
   :: sibenice-win64 / -winArm64 / -win32 / -...
   set bin_nazev=test-win

:: parametry pro prekladac MSVC
   :: debug:    /D_CRT_SECURE_NO_WARNINGS /W4 /Od /Zi /DEBUG /fsanitize=address
   :: release:  /nologo /D_CRT_SECURE_NO_WARNINGS /W2 /O2
   set cc_param=/nologo /D_CRT_SECURE_NO_WARNINGS /W2 /O2

:: parametry pro Resource Compiler
   set res_param=/nologo

:: parametry pro TERM_SET na Windows
   :: TERM_SET == 1
      :: /link /subsystem:windows /entry:mainCRTStartup user32.lib
   :: TERM_SET == 0
      :: /link /subsystem:console
   set term_set=/link /subsystem:windows /entry:mainCRTStartup user32.lib

:: prikaz pro spusteni prekladace vc. parametru (cl/clang apod.)
   set cc=cl.exe

:: korenovy adresar se zdrojovymi kody
   set src_dir=.\source

:: korenovy adresar pro binarni soubory
   set out_dir=.\bin

:: resource soubory
   set res_dir=.\res
   set vs_version_info=win-versioninfo


:: oramovani (pouze pro vystup skriptu)
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
:: zkopiruje info a napovedu do slozky bin
   copy %src_dir%\..\res\readme.md %out_dir%
   copy %src_dir%\..\res\napoveda.md %out_dir%

:: kompilace jednotlivych souboru

:: resource files
   rc.exe %res_param% %res_dir%\%vs_version_info%.rc
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
:: slozka libs
   cd %src_dir%\libs
   %cc% %cc_param% /c *.c
   cd ..\..
   move %src_dir%\libs\*.obj %out_dir%
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
   %cc% %cc_param% /Fe%bin_nazev%.exe *.obj %vs_version_info%.res %term_set%
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
