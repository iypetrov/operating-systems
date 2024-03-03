#!/bin/bash

data="$(cat "input.txt" | awk -F ':' '{printf("%s:%s:%s\n", $1, $5, $6)}' | grep "home/Inf" | awk -F ':' '{printf("%s %s\n", $1, $2)}' | tr ',' ' ' | awk '{printf("%s %s %s\n", $1, $2, $3)}')"
fns="$(echo "$data" | grep -P "a$" | cut -d' ' -f1)"
echo "$fns" | cut -c 3-4 | sort -rn | uniq -c | head -n 1 | sed 's/^[[:space:]]*//'
