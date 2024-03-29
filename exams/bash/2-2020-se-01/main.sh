#!/bin/bash

if [[ $# -ne 2 ]]; then
    echo "provide 2 args"
    exit 99
fi

if ! [[ "$1" =~ \.csv$ ]]; then
    echo "1 arg should be a name for .csv file"
    exit 99
fi

if ! [[ -d "$2" ]]; then
    echo "2 arg should be a dir"
    exit 99
fi

CSV="$1"
SRC="$2"

echo "hostname,phy,vlans,hosts,failover,VPN-3DES-AES,peers,VLAN Trunk Ports,license,SN,key" > "$CSV"

while read file; do
    fname="$(basename "$file" \
        | rev \
        | cut -d '.' -f2- \
        | rev)"
    data="$(cat "$file" \
        | tail -n +2 \
        | sed '/^$/d' \
        | sed 's/^[ \t]*//g')"
    license="$(echo "$data" \
        | sed -n '8p' \
        | cut -d ' ' -f5- \
        | rev \
        | cut -d ' ' -f2- \
        | rev)"
    fields="$(echo "$data" \
        | sed '8d' \
        | cut -d ':' -f2 \
        | sed 's/^ \+//g' \
        | sed "8i${license}" \
        | sed "1i${fname}" \
        | tr '\n' ',' \
        | head -c -1)"
    echo "$fields" >> "$CSV"
done < <(find "$SRC" -type f -name *.log)
