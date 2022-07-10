#!/bin/sh

######################################
#                                    #
#  SKRIPT NA SESTAVENÍ HRY ŠIBENICE  #
#  (BASH/LINUX)                      #
#                                    #
#  autor:  Martin TÁBOR (Ma-TA)      #
#  datum:  2022-07-10                #
#                                    #
######################################


######################################
# KONFIGURACE:
######################################


# úroveň ladění:
#   0 - vypnuto
#   1 - obrazovka není mazána
#   2 - hádá se jedno přednastavené slovo
  DBGLEVEL=0

# verze programu
  VERZE='0.0.2'

# název spustitelného souboru
  bin_nazev='sibenice_'$VERZE

# parametry překladače
    # debug:   '-Wall -Wextra -pedantic -g -fsanitize=address -DVERZE=\"'$VERZE'\" -DDEBUG='$DBGLEVEL
    # release: '-DVERZE=\"'$VERZE'\" -DDEBUG='$DBGLEVEL
  cc_param='-Wall -Wextra -pedantic -g -fsanitize=address -DVERZE=\"'$VERZE'\" -DDEBUG='$DBGLEVEL

# kořenový adresář se zdrojovými kódy
  src_dir=`pwd`'/source'

# kořenový adresář pro binární soubory
  out_dir=`pwd`'/bin'


######################################


# příkaz pro spuštění překladače vč. parametrů
alias cc='gcc '$cc_param

clear

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
# složka game/ukazatele
cd $src_dir'/game/ukazatele'
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
# složka tui
cd $src_dir'/tui'
cc -c *.c
mv *.o $out_dir

# sestavení spustitelného souboru
cd $out_dir
cc *.o -o $bin_nazev
rm *.o

# spuštění sestaveného programu
$out_dir/$bin_nazev -v
