#!/bin/bash

if [[ ! $(id -u) -eq 0 ]]; then
    echo "run the command as root user"
    exit 99
fi

if [[ ! "$#" -eq 1 ]]; then
    echo "provide 1 arg"
    exit 99
fi

if [[ ! "$1" =~ ^[0-9]+$ ]]; then
    echo "the arg should be a number"
    exit 99
fi

lim_rss=$1

data="$(ps -e -o uid,pid,rss | tail -n +2 | tr -s ' ' | sed 's/^[[:space:]]*//')"
while read line; do
    user_data="$(echo "$data" | awk -v USR="$line" '{if($1==USR) {printf "%s\n", $0}}')" 
    sum_rss="$(echo "$user_data" | awk 'BEGIN{rss=0}{rss+=$3}END{print rss}')"
    max_rss_pid="$(echo "$user_data" | sort -t ' ' -k 3 -nr | head -n +1 | cut -d ' ' -f3)"

    if [[ $lim_rss -lt $sum_rss ]]; then
        kill -9 "$max_rss_pid"
    fi
done < <(echo "$data" | cut -d' ' -f1 | sort -nu)
