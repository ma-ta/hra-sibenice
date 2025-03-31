#!/usr/bin/env bash

# Konce radku v souboru musi byt reprezentovany znakem LF ( !! nikoli CRLF !! )
# Testovane OS: Ubuntu 24.04.2 LTS, Fedora 42, macOS 15.4, FreeBSD 14.2

######################################
#                                    #
#  SKRIPT NA SESTAVENI HRY SIBENICE  #
#  (UN*X / BASH|ZSH / GCC|CLANG)     #
#                                    #
#  autor:  Martin TABOR (Ma-TA)      #
#  datum:  2025-03-31                #
#                                    #
######################################


######################################
# KONFIGURACE:
######################################


# nazev spustitelneho souboru
  bin_nazev='sibenice'        # -linux64 / -linuxArm64 / -linux32 /
                              # -mac64 / -macArm64 / -...

# parametry prekladace
  # debug:   '-Wall -Wextra -Wpedantic -Wno-deprecated-declarations -g -fsanitize=address'
  # release: '-O2 -Wno-unused-result'
  cc_param='-Wall -Wextra -Wpedantic -Wno-deprecated-declarations -g'

  # prikaz pro spusteni prekladace vc. parametru (cc/gcc/clang apod.)
  CC='cc '$cc_param  # (pro cross-kompilaci řádek za-komentovat)

  # cross-kompilace pod Ubuntu 24.04 LTS x86-64 (od-komentovat prislusne radky nize)
  # AArch64:
    #sudo apt install gcc-aarch64-linux-gnu -y
    #CC='aarch64-linux-gnu-gcc '$cc_param
  # x86 (i386):
    #sudo apt install gcc-multilib
    #CC='gcc -m32 '$cc_param

# korenovy adresar se zdrojovymi kody
  src_dir=`pwd`'/source'

# korenovy adresar pro binarni soubory
  out_dir=`pwd`'/bin'

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
  cp -R $src_dir'/data' $out_dir
# zkopiruje info a napovedu do slozky bin
  cp $src_dir'/../res/info.txt' $out_dir
  cp $src_dir'/../res/napoveda.txt' $out_dir

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
  ${CC} *.o -o $bin_nazev
  rm *.o


# spusteni sestaveneho programu
  echo ''
  echo $oramovani
  echo 'Spoustim:'
  echo "${out_dir}/${bin_nazev}"
  echo $oramovani
  echo ''

  $out_dir/$bin_nazev -v

# vyckani na stisk klavesy
  echo ''
  echo $oramovani
  echo ''
  echo '(stiskni Enter...)'
  read INPUT
