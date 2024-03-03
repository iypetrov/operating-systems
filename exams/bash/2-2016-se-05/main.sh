#!/bin/bash

if [[ "$#" -ne 2 ]]; then
    echo "provide 2 args"
    exit 99
fi

if ! [[ $1 =~ ^[0-9]+$ ]]; then
    echo "arg 1 should be a number"
    exit 99
fi

if ! [[ $2 =~ ^[0-9]+$ ]]; then
    echo "arg 2 should be a number"
    exit 99
fi

input_dir="input"

rm -rf "$input_dir/a" 2> /dev/null
rm -rf "$input_dir/b" 2> /dev/null
rm -rf "$input_dir/c" 2> /dev/null

mkdir -p "$input_dir/a"
mkdir -p "$input_dir/b"
mkdir -p "$input_dir/c"

while read line; do
    line_number="$(cat "$line" | wc -l)"
    if [[ $line_number -lt $1 ]]; then
        mv "$line" "$input_dir/a/$(basename "$line")"
    elif [[ $line_number -gt $2  ]]; then
        mv "$line" "$input_dir/c/$(basename "$line")"
    else
        mv "$line" "$input_dir/b/$(basename "$line")"
    fi
    echo "$(basename "$line")"
done < <(find "$input_dir" -type f)
