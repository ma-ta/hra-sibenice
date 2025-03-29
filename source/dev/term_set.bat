@echo off
@REM Ad-hoc pomůcka pro sestavování term_set.c
@REM na Windows (vč. přepínačů a závislostí)
@REM (v term_set.c nastavit DEBUG na 1 pro test modulu)

@REM Pro kompilaci term_main.c a term_set.c zvlášt (test použití jako modulu programu)
@REM cl.exe /c term_set.c
@REM cl.exe *.obj term_main.c /Feterm_win.exe /link /subsystem:windows /entry:mainCRTStartup user32.lib

@REM Samostatná kompilace modulu (DEBUG == 1) - obsahuje fci. main() s testy
cl.exe term_set.c /Feterm_win.exe /link /subsystem:windows /entry:mainCRTStartup user32.lib

del *.obj
