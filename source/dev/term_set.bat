@echo off
@REM Ad-hoc pomůcka pro sestavování term_set.c
@REM na Windows (vč. přepínačů a závislostí)
@REM (v term_set.c nastavit DEBUG na 1 pro test modulu)

cl.exe /c term_set.c /link /subsystem:windows /entry:mainCRTStartup user32.lib
cl.exe term_main.c term_set.obj /Feterm_win.exe
del *.obj
