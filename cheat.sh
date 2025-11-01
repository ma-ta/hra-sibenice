#!/usr/bin/env bash

# Konce radku v souboru musi byt reprezentovany znakem LF ( !! nikoli CRLF !! )
# Prerekvizity: GNU gdb

######################################
#                                    #
#  CHEAT PRO VYPSANI AKTUALNE        #
#  HADANEHO TAJNEHO SLOVA            #
#                                    #
#  V prubehu hry spustit:            #
#  ./cheat.sh <nazev_procesu>        #
#                                    #
######################################

# Verze        :  Hra Šibenice v1.3.2
# Testovane OS :  Ubuntu 24.04.3 LTS, Fedora 43
# Autor        :  Martin TABOR (Ma-TA)
# Upraveno     :  2025-11-01


# kontrola pritomnosti argumentu <nazev_procesu>
# (jinak se zkusi implicitni nazev "test-unix")
if [ -z "$1" ]; then
    echo "Pouziti: $0 <nazev_procesu>"
    echo
fi

PID=$(pgrep -fo "${1:-test-unix}")

# kontrola, zda promenna PID neni nulove delky
if [[ -n "$PID" ]]; then
    echo pid: $PID
    # vycteni retezce hadane_slovo->slovo z pameti RAM
    # (funguje, protoze retezec je prvni polozka struktury)
    sudo gdb --batch -p $PID -ex 'x/s &hadane_slovo' -ex quit | grep hadane_slovo
else
    # ukonceni v pripade, ze proces s danym nazvem neexistuje
    echo "(!) Proces nebyl nalezen..." >&2
    exit 1
fi
