#!/bin/bash

CFG="$1"
KEY="$2"
VAL="$3"

entries="$(cat "$CFG" \
        | cut -d '#' -f1 \
        | sed '/^$/d' \
        | sed 's/[ \t]\+//g' \
        | tee )"
echo "entries:"
echo "$entries"

if echo "$entries" | cut -d '=' -f1 | grep -qs "$KEY"; then
    i=$(cat "$CFG" \
        | grep -n -E "^[ \t]*${KEY}[ \t]*=[ \t]*.*$" \
        | cut -d ':' -f1 \
        | tee )
    echo "i -> $i"

    old_line="$(cat "$CFG" \
        | sed -n "${i}p" \
        | tee )"
    echo "old_line -> $old_line"

    comm_line="$(echo "# $old_line # edited at $(date) by $(whoami)")"
    sed -i "${i}c ${comm_line}" "$CFG"
    echo "comm_line -> $comm_line"

    new_line="$(echo "$KEY = $VAL # added at $(date) by $(whoami)")"
    sed -i "${i}a ${new_line}" "$CFG"
    echo "new_line -> $new_line"
else
    echo "$KEY = $VAL # added at $(date) by $(whoami)" >> "$CFG"
fi
