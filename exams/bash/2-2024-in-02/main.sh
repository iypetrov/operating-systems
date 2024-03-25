#!/bin/bash

info_h_file () {
    line="$(cat "$1" | grep class | head -n 1)"
    node="$(echo $line \
        | cut -d ':' -f1 \
        | sed 's/class//' \
        | sed 's/[[:space:]]//g')"
    echo "$node"
    
    if [[ "$line" =~ [:] ]]; then
        pnodes="$(echo "$line" \
            | cut -d ':' -f2 \
            | sed 's/public//' \
            | sed 's/private//' \
            | sed 's/protected//' \
            | tr -d ',' \
            | sed 's/ \+/ /g' \
            | sed 's/^[[:space:]]*//g')"
            
        echo "$pnodes" | tr ' ' '\n'
        echo "$pnodes" \
            | xargs -n 1 printf "%s\n" \
            | awk -v NODE="$node" '{printf "%s -> %s\n", $0, NODE}'
    fi
}

if [[ $# -ne 2 ]]; then
    echo "provide 2 args"
    exit 99
fi

if ! [[ -d "$1" ]]; then
    echo "1 arg should be a dir"
    exit 99
fi

DIR="$1"
GRAPH_PATH="$2"

while read file; do
    info_h_file "$file"
done < <(find "$DIR" -type f -name *.h) | sort | uniq
