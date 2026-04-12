#!/usr/bin/env sh

# SKRIPT NEFUNGUJE U STRIPPED BINAREK

# Konce radku v souboru musi byt reprezentovany znakem LF (nikoli CRLF)
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

# Verze        :  Hra Šibenice v1.3.3
# Testovane OS :  Ubuntu 26.04 LTS, Fedora 44
# Autor        :  Martin TABOR (Ma-TA)
# Upraveno     :  2026-04-05


NAZEV="$1"  # nazev procesu

B_CERVENA="\e[31m"  # ANSI - text cervene barvy
B_MODRA="\e[34m"    # ANSI - text modre barvy
B_VYCHOZI="\e[39m"  # ANSI - text vychozi barvy


# kontrola pritomnosti argumentu <nazev_procesu>
# (jinak se zkusi implicitni nazev "test-unix")
if [ -z "$NAZEV" ]; then
    echo -en "$B_CERVENA"  # cerveny text
    echo "POUZITI SKRIPTU: $0 <nazev_procesu>"
    echo -en "$B_VYCHOZI"  # reset barvy textu
    echo

    NAZEV="test-unix"  # vychozi nazev
    echo "Zkusim vyhledat proces: $NAZEV"
fi

# hledani PID nejnovejsiho procesu s danym nazvem
PID=$(pgrep -n "$NAZEV")

# kontrola, zda promenna PID neni nulove delky
if [ -n "$PID" ]; then
    echo "Nalezeno PID: $PID... Spoustim GDB:"

    # vycteni retezce hadane_slovo->slovo z pameti RAM
    # (funguje, protoze retezec je prvni polozka struktury)
    echo -en "$B_MODRA"  # cerveny text
    gdb --batch -p "$PID" -ex "x/s &hadane_slovo" -ex quit | grep hadane_slovo
    echo -en "$B_VYCHOZI"  # reset barvy textu

else
    # ukonceni v pripade, ze proces s danym nazvem neexistuje
    echo -en "$B_CERVENA"  # cerveny text
    echo "(!) Proces \"$NAZEV\" nebyl nalezen..." >&2
    echo -en "$B_VYCHOZI"  # reset barvy textu

    exit 1
fi
