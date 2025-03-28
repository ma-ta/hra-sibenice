@echo off
@REM Ad-hoc pomůcka pro sestavování term_set.c
@REM na Windows (vč. přepínačů a závislostí)
@REM (v term_set.c nastavit DEBUG na 1 pro test modulu)

cl.exe /c term_set.c
cl.exe *.obj term_main.c /Feterm_win.exe /link /subsystem:windows /entry:mainCRTStartup user32.lib
del *.obj
