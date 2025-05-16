#!/usr/bin/env bash

# Konce radku v souboru musi byt reprezentovany znakem LF ( !! nikoli CRLF !! )
# Testovane prostredi pro sestaveni:
#   - Ubuntu 24.04.2 LTS + Emscripten v4.0.8,
#   - npm v11.3.0, xterm v5.5.0, xterm-pty v0.11.1

######################################
#                                    #
#  SKRIPT NA SESTAVENI HRY SIBENICE  #
#  (UN*X / BASH|ZSH / EMSCRIPTEN     #
#                                    #
#  autor:  Martin TABOR (Ma-TA)      #
#  datum:  2025-05-16                #
#                                    #
######################################


######################################
# KONFIGURACE:
######################################

# ulozeni vychoziho adresare
  top_dir=`pwd`

# nazev spustitelneho souboru
  # -wasm.js / -wasm.mjs / -wasm.html
  bin_nazev='sibenice-wasm.mjs'

# korenovy adresar se zdrojovymi kody (C)
  c_dir=$top_dir'/source'

# korenovy adresar se zdrojovymi kody (WEB)
  web_dir=$top_dir'/source-web'

# korenovy adresar pro sestaveni
  out_dir=$top_dir'/bin/wasm'

# parametry prekladace
  cc_param='-O3'

# parametry pri linkovani *.o
  link_param='-s ASYNCIFY
    -s WASM=1
    -s EXIT_RUNTIME=1
    -s FORCE_FILESYSTEM=1'

  # prikaz pro spusteni prekladace vc. parametru (emcc apod.)
  CC='emcc '$cc_param

  # cesta k souborum, ktere maji byt zahrnuty ve virtualnim FS
  # (pozor: v [../../source/data/game_cs.dat] nejsou slova sifrovana)
  fs_data='--preload-file ../../bin/data@data'

  # cesta ke JS-knihovnam (emscripten-pty.js apod.)
  js_libs='--js-library=../../source-web/node_modules/xterm-pty/emscripten-pty.js'


# oramovani (pouze pro vystup skriptu)
  oramovani='----------------------------------'


######################################
# ČÁST 1 - vytvoreni webove casti, npm
######################################

clear

echo $oramovani
echo 'VYTVARIM WEB - npm, frontend atd.:'
echo $oramovani
echo ''

# korenovy adresar se zdrojovymi kody
  src_dir=$web_dir

# korenovy adresar pro binarni soubory
  out_dir=$out_dir

# overeni a aktualizace zavislosti
  cd $src_dir
  ./make_web.sh
  cd $top_dir

# kopirovani souboru pro frontend
  cp -R -p $src_dir'/.' $out_dir

# vyckani na stisk klavesy
  echo ''
  echo $oramovani
  echo ''
  echo '(stiskni Enter...)'
  read INPUT


######################################
# ČÁST 2 - kompilace Wasm s xterm-pty:
######################################

echo $oramovani
echo 'KOMPILUJI - Emscripten, Wasm:'
echo $oramovani
echo ''

# korenovy adresar se zdrojovymi kody
  src_dir=$c_dir

# korenovy adresar pro binarni soubory
  out_dir=$out_dir

# vytvori adresar bin pro vystupy
  mkdir -p $out_dir
# zkopiruje slozku data do slozky bin
  #cp -Ri $src_dir'/data' $out_dir
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
  ${CC} $link_param $fs_data $js_libs *.o -o $bin_nazev
  rm *.o

# vyckani na stisk klavesy
  echo ''
  echo $oramovani
  echo ''
  echo '(stiskni Enter...)'
  read INPUT
