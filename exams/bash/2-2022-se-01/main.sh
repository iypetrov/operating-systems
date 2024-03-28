#!/bin/bash

update_wake_state () {
    DEVICE="$1"
    STATE="$2"

    if [[ "$DEVICE" = "DEVICE" ]]; then
        echo "can not give DEVICE as device" 
        return 99
    fi

    devices="$(cat "input.txt" \
        | tail -n +2 \
        | sed 's/[[:space:]]\+/ /g' \
        | cut -d ' ' -f1)"

    if ! echo "$devices" | grep -qsx "$DEVICE"; then
        echo "this device doesn't exist"
        return 99
    fi

    index="$(cat "input.txt" \
        | awk -v TRG="$DEVICE" '{cnt++; if (TRG == $1) { printf "%d %s\n", cnt, $0}}' \
        | cut -d ' ' -f1)"
    line="$(cat "input.txt" \
        | sed -n "${index}p" \
        | awk -v DIS="$STATE" '{printf "%s\t%s\t%s\t%s\n", $1, $2, DIS, $4}')"
    data="$(awk -v NUM="$index" -v LINE="$line" '{cnt++; if(cnt==NUM) {print LINE} else {printf "%s\t%s\t%s\t%s\n", $1, $2, $3, $4}}' input.txt)"

    echo "$data" > input.txt
    echo "updated $DEVICE to $STATE"
}

if [[ $# -ne 1 ]]; then
    echo "provide at least 1 arg" 
    exit 99
fi

if ! [[ -f "$1" ]]; then
    echo "1 arg should be a file" 
    exit 99
fi

i=0
while read line; do
    i=$((i + 1))
    data="$(echo "$line" \
        | sed 's/[[:space:]]\+/ /g' \
        | cut -d '#' -f1)"
    nf="$(echo "$data" \
        | tr ' ' '\n' \
        | wc -l)"

    if [[ $nf -lt 2 ]]; then
        echo "nothing to scan on line $i" 
    else
        device="$(echo "$data" | cut -d ' ' -f1)"
        state="$(echo "$data" | cut -d ' ' -f2)"
        update_wake_state "$device" "$state"
    fi

done < <(cat "$1")
