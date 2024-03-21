#!/bin/bash

get_cnt_line() {
    cat "$1" | head -n $2 | tail -n 1 | cut -d '=' -f2 | tr ' ' '\n' | sort | uniq
}

line_one="$(cat "$1" | cut -d '=' -f1 | grep -nx "$2" | cut -d ':' -f1)" 
line_two="$(cat "$1" | cut -d '=' -f1 | grep -nx "$3" | cut -d ':' -f1)" 

nline=""
while read line; do
    if ! echo "$line" | grep -qs "$(get_cnt_line "$1" "$line_one")"; then
        nline+="$line "
    fi
done < <(get_cnt_line "$1" "$line_two")

fs="$(echo "$3=$nline")"
sed -i "${line_two}s/.*/$fs/" "$1"
