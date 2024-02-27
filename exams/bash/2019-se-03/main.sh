#!/bin/bash

get_depth_file() {
    echo "$1" | tr -dc "/" | wc -c
}

in_last="$(find ~ -maxdepth 5 -type f -print0 | xargs -0 stat --format "%i %n %Z" | sort -t ' ' -k 3 -nr | head -n +1 | cut -d ' ' -f1)"

while read line; do
    dth="$(get_depth_file $line)"
    echo "$line $dth"
done < <(find ~ -maxdepth 5 -type f -print0 | xargs -0 stat --format "%i %n" | awk -v LST="$in_last" '{ if($1 == LST) {printf "%s\n", $2}}') | sort -t ' ' -k 2 -n | head -n +1 | cut -d ' ' -f1
