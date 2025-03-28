@echo off
@REM Ad-hoc pomůcka pro sestavování term_set.c
@REM na Windows (vč. přepínačů a závislostí)

cl.exe term_set.c /link /subsystem:windows /entry:mainCRTStartup user32.lib
del *.obj
term_set.exe 25 25
