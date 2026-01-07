#!/usr/bin/env sh

# Konce radku v souboru musi byt reprezentovany znakem LF ( !! nikoli CRLF !! )

# Testovane OS: Ubuntu 24.04.3 LTS, Fedora 43

# Skript umožňuje cross kompilaci MS-DOS binárky pomocí Open Watcom v Ubuntu
# a je vhodný spíše pro testovací sestavení - např. za využití Ubuntu WSL
# a DOSBox-X (pro finální binárku lépe využít FreeDOS - viz [mk_dos16.bat]).
# Prerekvitizy:
#   Ubuntu: [sudo snap install open-watcom --beta]
#   Fedora: [sudo dnf install snap && sudo snap install open-watcom --beta]

######################################
#                                    #
#  SKRIPT NA SESTAVENI HRY SIBENICE  #
#  (UBUNTU / BASH / OPEN WATCOM)     #
#                                    #
#  autor:  Martin TABOR (Ma-TA)      #
#  datum:  2026-01-07                #
#                                    #
######################################


######################################
# KONFIGURACE:
######################################


# nazev spustitelneho souboru
  # test-dos.exe
  bin_nazev='test-dos.exe'

# parametry prekladace
  # velikosti zasobniku pro ow (8 KB == Stack Overflow!)
  # (hra vyzaduje vetsi zasobnik nez je vychozi u Open Watcom)
  cc_param='-k 16384'

# prikaz pro spusteni prekladace vc. parametru (cc/gcc/clang apod.)
  # Ubuntu: '/snap/bin/open-watcom.owcc-dos '
  # Fedora: 'open-watcom.owcc-dos '
  CC='open-watcom.owcc-dos '$cc_param

# korenovy adresar se zdrojovymi kody
  src_dir=`pwd`'/source'

# korenovy adresar pro binarni soubory
  out_dir=`pwd`'/bin'


# oramovani (pouze pro vystup skriptu)
  oramovani='----------------------------------'


######################################


clear

echo $oramovani
echo 'SESTAVUJI - Vypis chyb a udalosti:'
echo $oramovani
echo ''

# vytvori adresar bin pro vystupy
  mkdir -p $out_dir
# zkopiruje slozku data do slozky bin
  echo "Nahrazeni puvodnich souboru [y/N]:"
  cp -Ri $src_dir'/data' $out_dir
# zkopiruje info a napovedu do slozky bin
  cp $src_dir'/../res/readme.md' $out_dir
  cp $src_dir'/../res/napoveda.md' $out_dir

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


# informace o vytvorene binarce
  echo ''
  echo $oramovani
  echo ''
  echo "Informace o vytvorenem souboru [file ${bin_nazev}]:"
  echo ''
  file -b $out_dir/$bin_nazev | sed 's/, */,\n/g'
  echo ''

# spusteni sestaveneho programu
  echo $oramovani
  echo ''
  echo "Spoustim [./${bin_nazev} -v]:"
  echo ''

  $out_dir/$bin_nazev -v


# vyckani na stisk klavesy
  echo ''
  echo $oramovani
  echo ''
  echo '(stiskni Enter...)'
  read INPUT
