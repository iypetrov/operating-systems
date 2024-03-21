#!/bin/bash

depth_lvl () {
    realpath "$1" | tr '/' '|' | grep -o '|' | wc -l
}

if [[ $# -ne 3 ]]; then
    echo "provide 3 args"
    exit 99
fi    

if ! [[ -d "$(realpath "$1")" ]]; then
    echo "1st arg should be a dir"
    exit 99
fi    

if ! [[ -d "$(realpath "$2")" ]]; then
    echo "2nd arg should be a dir"
    exit 99
fi    

if [[ $(find "$(realpath "$2")" -type f | wc -l) -ne 0 ]]; then
    echo "2nd arg should be a non-empty dir"
    exit 99
fi    

if [[ $(id -u) -ne 0 ]]; then
    echo "should run the script as root"
    exit 99
fi

SRC="$(realpath $1)"
d_src=$(depth_lvl "$SRC")
DST="$(realpath $2)"
ABC="$3"

while read line; do
    rel_path="$(echo "$line" | awk -F '/' -v CNT=$((d_src + 1)) '{for(i=CNT; i<=NF; i++) if(i!=NF) {printf "%s/", $i }else{ printf "%s", $i}; print "\n"}')"
    depth=$(echo "$rel_path" | tr '/' '|' | grep -o '|' | wc -l)
    if [[ $depth -gt 0 ]]; then
        mkdir -p "$DST/$(echo "$rel_path" | cut -d '/' -f1-$depth)"
    fi
    cp -p "/$line" "$DST/$rel_path"
done < <(find "$SRC" -type f -name *"$ABC"* | awk -F '/' '{for (i=2; i<=NF;i++) if(i!=NF) {printf "%s/", $i }else{ printf "%s", $i}; print "\n"}' | grep -v '^$')
