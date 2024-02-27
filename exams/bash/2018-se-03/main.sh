#!/bin/bash

while read line; do
    fn="$(echo "$line" | cut -d : -f1 | tr -d s)"
    name="$(echo "$line" | cut -d : -f2 | cut -d ',' -f1)"
    path="$(echo "$line" | cut -d : -f3)"
    echo "$fn $name $path"
done < <(cat input.txt | awk -F ':' '{printf "%s:%s:%s\n", $1, $5, $6}') | sort -t ' ' -k1 -n | awk '{printf "%s %s:%s\n", $2, $3, $4}' 
