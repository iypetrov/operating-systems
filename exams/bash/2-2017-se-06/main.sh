#!/bin/bash

data="$(ps -e -o uid,pid,rss \
    | sed 's/^[[:space:]]*//g' \
    | sed 's/ \+/ /g' \
    | tail -n +2)"
result="$(while read uid; do
    echo "$data" | awk -v UID="$uid" '{if ($1 == UID) printf "%s\n", $0 }' \
        | awk 'BEGIN {cnt=0} {cnt=cnt+$3} END {printf "%s %s\n",$1,cnt}'
done < <(echo "$data" | \
        cut -d ' ' -f1 | \
        sort -n | uniq))"

lim="$(echo "$result" | grep -E '^0 ' | cut -d ' ' -f2)"

while read uid; do
    echo "$data" | awk -v FOO="$uid" '{if ($1 == FOO) {print $2}}' | kill -9
done < <(echo "$result" | awk -v LIM="$lim" '{if (LIM < $2) { print $1}}')
