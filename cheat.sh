#!/usr/bin/env bash

# Konce radku v souboru musi byt reprezentovany znakem LF ( !! nikoli CRLF !! )

# Cheat pro vypsani aktualne hadaneho slova
# Pouziti: ./cheat.sh <nazev_procesu>
# (spustit v prubehu hry)


if [ -z "$1" ]; then
    echo "Pouziti: $0 <nazev_procesu>"
    echo
fi

PID=$(pgrep -fo "${1:-test-unix}")

if [[ -n "$PID" ]]; then
    echo pid: $PID
    gdb --batch -p $PID -ex 'x/s &hadane_slovo' -ex quit | grep hadane_slovo
else
    echo "(!) Proces nebyl nalezen..." >&2
    exit 1
fi
