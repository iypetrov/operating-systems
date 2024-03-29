#!/bin/bash

info_site () {
    freqs="$1"
    site="$2"

    http_stat="$(echo "$freqs" \
        | awk -v SITE="$site" '{if ($2 == SITE) {print $0}}' \
        | cut -d ' ' -f7 \
        | sort \
        | uniq -c \
        | sed 's/^[ \t]*//g')"

    http1=$(echo "$http_stat" \
        | grep -v -E '^[0-9]+ HTTP\/2\.0.*$' \
        | cut -d ' ' -f1)
    if [[ -z "$http1" ]]; then
        http1=0
    fi

    http2=$(echo "$http_stat" \
        | grep -E '^[0-9]+ HTTP\/2\.0.*$' \
        | cut -d ' ' -f1)
    if [[ -z "$http2" ]]; then
        http2=0
    fi

    echo "$site HTTP/2.0: $http2 non-HTTP/2.0: $http1"
}

SRC="$1"

top_sites="$(cat "$SRC" \
    | cut -d ' ' -f2 \
    | sort \
    | uniq -c \
    | sed 's/^[ \t]*//g' \
    | sort -t ' ' -k 1 -nr \
    | head -n 3 \
    | cut -d ' ' -f2)"

freqs="$(while read log; do
    site="$(echo "$log" \
        | cut -d ' ' -f2)"
    if echo "$top_sites" | grep -qs "$site"; then
        echo "$log"
    fi
done < <(cat "$SRC"))"

while read site; do
    info_site "$freqs" "$site"
done < <(echo "$top_sites")

echo "$freqs" \
    | awk '{if($8 >= 302) {print $0}}' \
    | cut -d ' ' -f1 \
    | sort -n \
    | uniq -c \
    | head -n 5
