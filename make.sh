#!/bin/bash

# Konce řádků souboru musí být reprezentovány znakem LF (nikoli CRLF)
# Testované OS: Fedora 39, Ubuntu 22.04 LTS

######################################
#                                    #
#  SKRIPT NA SESTAVENÍ HRY ŠIBENICE  #
#  (UN*X/Bash/GCC+Clang              #
#                                    #
#  autor:  Martin TÁBOR (Ma-TA)      #
#  datum:  2024-03-11                #
#                                    #
######################################


######################################
# KONFIGURACE:
######################################


# název spustitelného souboru
  bin_nazev='sibenice_linux'

# parametry překladače
  # debug:   '-Wall -Wextra -pedantic -g -fsanitize=address'
  # release: '-O2'
  cc_param='-O2'

  # příkaz pro spuštění překladače vč. parametrů (cc/gcc/clang apod.)
  CC='cc '$cc_param

# kořenový adresář se zdrojovými kódy
  src_dir=`pwd`'/source'

# kořenový adresář pro binární soubory
  out_dir=`pwd`'/bin'

# orámování
  oramovani='----------------------------------'


######################################


clear

echo $oramovani
echo 'SESTAVUJI - Vypis chyb a udalosti:'
echo $oramovani
echo ''

# vytvoří adresář bin pro výstupy
  mkdir $out_dir
# zkopíruje složku data do složky bin
  cp $src_dir'/data' -R $out_dir
# zkopíruje info a nápovědu do složky bin
  cp $src_dir'/../res/info.txt' $out_dir
  cp $src_dir'/../res/napoveda.txt' $out_dir

# kompilace jednotlivých souborů

# kořenový adresář
  cd $src_dir
  ${CC} -c *.c
  mv *.o $out_dir
# složka game
  cd $src_dir'/game'
  ${CC} -c *.c
  mv *.o $out_dir
# složka game/game_tui
  cd $src_dir'/game/game_tui'
  ${CC} -c *.c
  mv *.o $out_dir
# složka help
  cd $src_dir'/help'
  ${CC} -c *.c
  mv *.o $out_dir
# složka menu
  cd $src_dir'/menu'
  ${CC} -c *.c
  mv *.o $out_dir
# složka stats
  cd $src_dir'/stats'
  ${CC} -c *.c
  mv *.o $out_dir
# složka tui
  cd $src_dir'/tui'
  ${CC} -c *.c
  mv *.o $out_dir

# sestavení spustitelného souboru
  cd $out_dir
  ${CC} *.o -o $bin_nazev
  rm *.o

echo ''
echo $oramovani
echo ''

# spuštění sestaveného programu
  $out_dir/$bin_nazev -v

# vyčkání na stisk klávesy
  echo ''
  echo $oramovani
  echo ''
  echo '(stiskni Enter...)'
  read INPUT
