#!/bin/sh

######################################
#                                    #
#  SKRIPT NA SESTAVENÍ HRY ŠIBENICE  #
#  (BASH/LINUX)                      #
#                                    #
#  autor:  Martin TÁBOR (Ma-TA)      #
#  datum:  2022-07-18                #
#                                    #
######################################


######################################
# KONFIGURACE:
######################################


# název spustitelného souboru
  bin_nazev='sibenice'

# parametry překladače
  # debug:   '-Wall -Wextra -pedantic -g -fsanitize=address'
  # release: ''
  cc_param='-Wall -Wextra -pedantic -g -fsanitize=address'

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

# příkaz pro spuštění překladače vč. parametrů
  alias cc='gcc '$cc_param

# zkopíruje složku data do složky bin
  cp $src_dir'/data' -R $out_dir

# kompilace jednotlivých souborů

# kořenový adresář
  cd $src_dir
  cc -c *.c
  mv *.o $out_dir
# složka game
  cd $src_dir'/game'
  cc -c *.c
  mv *.o $out_dir
# složka game/game_tui
  cd $src_dir'/game/game_tui'
  cc -c *.c
  mv *.o $out_dir
# složka help
  cd $src_dir'/help'
  cc -c *.c
  mv *.o $out_dir
# složka menu
  cd $src_dir'/menu'
  cc -c *.c
  mv *.o $out_dir
# složka stats
  cd $src_dir'/stats'
  cc -c *.c
  mv *.o $out_dir
# složka tui
  cd $src_dir'/tui'
  cc -c *.c
  mv *.o $out_dir

# sestavení spustitelného souboru
  cd $out_dir
  cc *.o -o $bin_nazev
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
