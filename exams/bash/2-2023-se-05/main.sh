#!/bin/bash

snapshot () {
    cmds="$(ps -e -o cmd \
        | tail -n +2 \
        | sort \
        | uniq)"
    while read cmd; do        
        rss="$(ps -e -o cmd,rss \
            | tail -n +2 \
            | sed 's/ \+/ /g' \
            | grep -E "^$cmd [0-9]+$" \
            | rev \
            | cut -d ' ' -f1 \
            | rev \
            | tr ' ' '\n' \
            | awk '{sum+=$1} END{print sum}')"
        if [[ -n "$rss" ]]; then
            echo "$cmd $rss"
        fi
    done < <(echo "$cmds")
}

LIM=65536

cnt=0
stat=""
start="$(date +"%s")"
while true; do
    curr="$(date +"%s")"
    if [[ "$start" != "$curr" ]]; then
        data="$(snapshot)"
        max_rss="$(echo "$data" \
            | rev \
            | cut -d ' ' -f1 \
            | rev \
            | sort -rn \
            | head -n 1)"     

        if [[ $max_rss -lt $LIM ]]; then
            sum_stat="$(echo "$stat" \
                | sort \
                | uniq -c \
                | sed 's/^[[:space:]]*//g')"

            cnt=$((cnt / 2))
            echo "$sum_stat" \
                | awk -v LIM="$cnt" '{if ($1 > LIM) { printf "%s\n", $0 }}' \
                | rev \
                | cut -d ' ' -f1 \
                | rev

            exit 0
        fi

        report="$(echo "$data" \
            | awk -v LIM="$LIM" '{if(LIM >= $2) {printf "%s\n", $0}}' \
            | rev \
            | cut -d ' ' -f2- \
            | rev)" 

        stat+="$report"
        cnt=$((cnt + 1))

        start="$curr"
    fi
done
