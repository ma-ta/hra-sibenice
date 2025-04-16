#!/usr/bin/env bash

# Konce radku v souboru musi byt reprezentovany znakem LF ( !! nikoli CRLF !! )
# Testovane OS: Ubuntu 24.04.2 LTS + Emscripten v4.0.7

######################################
#                                    #
#  SKRIPT NA SESTAVENI HRY SIBENICE  #
#  (UN*X / BASH|ZSH / EMSCRIPTEN     #
#                                    #
#  autor:  Martin TABOR (Ma-TA)      #
#  datum:  2025-04-16                #
#                                    #
######################################


######################################
# KONFIGURACE:
######################################

# nazev spustitelneho souboru
  # -web / -nodejs / -...
  bin_nazev='sibenice-web.js'

# korenovy adresar se zdrojovymi kody
  src_dir=`pwd`'/source'

# korenovy adresar pro binarni soubory
  out_dir=`pwd`'/bin/wasm'

# parametry prekladace
  # debug (navíc):
    #  -g4  \
    #  -s ASSERTIONS=1
  # release:
    #  --preload-file data@data  \
    #  -s EXIT_RUNTIME=0  \
    #  -s WASM=1  \
    #  -s MODULARIZE=1  \
    #  -s EXPORT_ES6=1  \
    #  -s ENVIRONMENT="web"  \
    #  -s FORCE_FILESYSTEM=1
  cc_param=--preload-file data@data -g  \
    -s ASSERTIONS=1  \
    -s EXIT_RUNTIME=0  \
    -s WASM=1  \
    -s MODULARIZE=1  \
    -s EXPORT_ES6=1  \
    -s ENVIRONMENT="web"  \
    -s FORCE_FILESYSTEM=1

  # prikaz pro spusteni prekladace vc. parametru (emcc apod.)
  CC='emcc '$cc_param

# oramovani
  oramovani='----------------------------------'


######################################


clear

echo $oramovani
echo 'SESTAVUJI - Vypis chyb a udalosti:'
echo $oramovani
echo ''

# vytvori adresar bin pro vystupy
  mkdir $out_dir
# zkopiruje slozku data do slozky bin
  #cp -R $src_dir'/data' $out_dir
# zkopiruje info a napovedu do slozky bin
  #cp $src_dir'/../res/readme.md' $out_dir
  #cp $src_dir'/../res/napoveda.md' $out_dir

# kompilace jednotlivych souboru

# korenovy adresar
  cd $src_dir
  ${CC} -c *.c
  mv *.o $out_dir
# slozka game
  cd $src_dir'/game'
  ${CC} -c *.c
  mv *.o $out_dir
# slozka game/game_tui
  cd $src_dir'/game/game_tui'
  ${CC} -c *.c
  mv *.o $out_dir
# slozka help
  cd $src_dir'/help'
  ${CC} -c *.c
  mv *.o $out_dir
# slozka libs
  cd $src_dir'/libs'
  ${CC} -c *.c
  mv *.o $out_dir
# slozka menu
  cd $src_dir'/menu'
  ${CC} -c *.c
  mv *.o $out_dir
# slozka stats
  cd $src_dir'/stats'
  ${CC} -c *.c
  mv *.o $out_dir
# slozka tui
  cd $src_dir'/tui'
  ${CC} -c *.c
  mv *.o $out_dir

# sestaveni spustitelneho souboru
  cd $out_dir
  ${CC} *.o --preload-file ../../source/data@data -o $bin_nazev
  rm *.o


# vyckani na stisk klavesy
  echo ''
  echo $oramovani
  echo ''
  echo '(stiskni Enter...)'
  read INPUT
