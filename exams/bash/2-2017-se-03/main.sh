#!/bin/bash

if [[ $(id -u) -ne 0 ]]; then
    echo "run it as root"
    exit 99
fi

while read user; do
    sum_rss=0
    while read line; do
        cuser="$(echo "$line" | cut -d ' ' -f1)"
        if [[ "$cuser" = "$user" ]]; then
            rss=$(echo "$line" | cut -d ' ' -f2)
            sum_rss=$((sum_rss + rss))
        fi
    done < <(ps -e -o uid,rss | tail -n +2 | sed 's/ \+/ /g')
    echo "$user $sum_rss"
done < <(ps -e -o uid | tail -n +2 | sort -n | uniq -c | sed 's/^[[:space:]]*//g' | sed 's/ \+/ /g' | cut -d ' ' -f2)

while read user; do
    cntr=0
    sum_rss=0
    max_rss=0
    max_pid=0
    while read line; do
        cntr=$((cntr + 1))
        cuser="$(echo "$line" | cut -d ' ' -f1)"
        if [[ "$cuser" = "$user" ]]; then
            rss=$(echo "$line" | cut -d ' ' -f2)
            if [[ $rss -gt $max_rss ]]; then
                max_rss=$rss
                max_pid=$(echo "$line" | cut -d ' ' -f3)
            fi
            sum_rss=$((sum_rss + rss))
        fi
    done < <(ps -e -o uid,rss,pid | tail -n +2 | sed 's/ \+/ /g')
    tws_avg=$((2 * sum_rss / cntr))
    if [[ $tws_avg -lt $max_rss ]]; then
        kill -9 $max_pid
    fi
done < <(ps -e -o uid | tail -n +2 | sort -n | uniq -c | sed 's/^[[:space:]]*//g' | sed 's/ \+/ /g' | cut -d ' ' -f2)
