#!/bin/bash

data="$(while read line; do
    fn="$(echo "$line" | cut -d : -f1 | tr -d s)"
    name="$(echo "$line" | cut -d : -f2 | cut -d ',' -f1)"
    path="$(echo "$line" | cut -d : -f3)"
    echo "$fn $name $path"
done < <(cat input.txt | awk -F ':' '{printf "%s:%s:%s\n", $1, $5, $6}') | grep /home/SI)"


while read line; do
    s_chng="$(echo "$line" | cut -d ' ' -f4 | xargs stat --format "%Z")"
    if [[ s_chng -ge 1709062214 && s_chng -le 1709062235 ]]; then
        echo "$line" | awk '{printf "%s\t%s\n", $1, $2}'
    fi
done < <(echo "$data")
