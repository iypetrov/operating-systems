#!/bin/bash

find input1 -type f -printf "%p %s\n" | awk '{ if($2 == 0) {print $1} }' | xargs rm -f 

while read line; do
    rmdir "$line" 2> /dev/null
done < <(find input1 -type d)
#data="$(while read line; do
#    n_dash="$(echo "$line" | grep -o / | tr -d '\n' | wc -m)"
#    echo "$line $n_dash"
#done < <(find input1 -type d) | sort -t ' ' -k 2 -nr)"
#
#max_depth="$(echo "$data" | head -n 1 | cut -d' ' -f2)"
#leaves="$(echo "$data" | awk -v foo="$max_depth" '{ if($2 == foo) print $1}')"
#
#while read line;do
#    rm -rf "$line"
#done < <(echo "$leaves")
