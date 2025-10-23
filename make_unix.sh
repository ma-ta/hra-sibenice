#!/usr/bin/env bash

# Konce radku v souboru musi byt reprezentovany znakem LF ( !! nikoli CRLF !! )

# Testovane OS: Ubuntu 24.04.3 LTS, Fedora 43, macOS 15.6, FreeBSD 14.3,
#               Android 14/Termux v0.119.0

######################################
#                                    #
#  SKRIPT NA SESTAVENI HRY SIBENICE  #
#  (UN*X / BASH|ZSH / GCC|CLANG)     #
#                                    #
#  autor:  Martin TABOR (Ma-TA)      #
#  datum:  2025-10-23                #
#                                    #
######################################


######################################
# KONFIGURACE:
######################################


# nazev spustitelneho souboru
  # sibenice-linux64 / -linux32  / -linuxArm64 / -linuxArm / -linuxRV64 / -...
  # -fbsd64  / -fbsd32   / -fbsdArm64  / -...
  # -mac64   / -macArm64 / -...
  # -android (Termux)    / -...
  bin_nazev='test-unix'

# parametry prekladace
  # debug:   '-Wall -Wextra -Wpedantic -Wno-deprecated-declarations -g -fsanitize=address'
  # release: '-O3 -Wno-unused-result'
  cc_param='-O3 -Wno-unused-result'  # pro testovani cross-kompilace pridat [-static]

  # pro macOS je mozne vyuzit prepinace:
    # [-arch arm64] pro Silicon, [-arch x86_64] pro Intel,
    # a nasledne vytvorit jedinou Universal binarku:
    # [lipo -create -output sibenice-macUni sibenice-macArm64 sibenice-mac64]

  # prikaz pro spusteni prekladace vc. parametru (cc/gcc/clang apod.)
  CC='cc '$cc_param

  # CROSS KOMPILACE:
    # cross-kompilace pod Ubuntu 24.04 LTS x86-64 (od-komentovat radky nize)
      # x86 (i386, 32-bit):
        #sudo apt install gcc-multilib
        #CC='gcc -m32 '$cc_param
      # AArch64 (ARM 64-bit):
        #sudo apt install gcc-aarch64-linux-gnu
        #CC='aarch64-linux-gnu-gcc '$cc_param
      # armhf (ARM 32-bit):
        #sudo apt install gcc-arm-linux-gnueabihf
        #CC='arm-linux-gnueabihf-gcc '$cc_param
      # riscv64 (RV64, RISC-V 64-bit):
        #sudo apt install gcc-riscv64-linux-gnu
        #CC='riscv64-linux-gnu-gcc '$cc_param
    # cross-kompilace pod FreeBSD 14.3 x86_64 (od-komentovat radky nize)
      # x86 (i386, 32-bit):
        #CC='clang -m32 '$cc_param
      # AArch64:
        #fetch https://download.freebsd.org/ftp/releases/arm64/14.3-RELEASE/base.txz
        #mkdir -p /opt/sysroot-aarch64
        #tar -xf base.txz --strip-components=1 -C /opt/sysroot-aarch64
        #CC='clang --target=aarch64-unknown-freebsd14.3 --sysroot=/opt/sysroot-aarch64'


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
