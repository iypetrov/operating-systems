#!/bin/bash

megaparsecs_bh () {
    SRC="$1"
    BH_NAME="$2"

    MEGAPARSECS="$(cat "$SRC" \
        | awk -F ':' -v TRG="$BH_NAME" '{if (TRG==$1) print $0}' \
        | cut -d ':' -f2 \
        | sed 's/megaparsecs//g' \
        | sed 's/ \+//g')"

    if [[ -z "$MEGAPARSECS" ]]; then
        echo 0
    else
        echo $MEGAPARSECS
    fi
}

if [[ $# -ne 3 ]]; then
    echo "provide 3 args"
    exit 99
fi

if ! [[ -f "$1" ]]; then
    echo "1 arg should be a file"
    exit 99
fi

if ! [[ -f "$2" ]]; then
    echo "2 arg should be a file"
    exit 99
fi

POINT_1="$1"
POINT_2="$2"
BH="$3"

MB_1="$(megaparsecs_bh "$POINT_1" "$BH")"
MB_2="$(megaparsecs_bh "$POINT_2" "$BH")"

if [[ $MB_1 -le $MB_2 ]]; then
    echo "$POINT_1"
else
    echo "$POINT_2"
fi
