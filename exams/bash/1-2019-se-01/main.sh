#!/bin/bash

type="$(cat "planets.txt" \
  | tail -n +2 \
  | sort -t ';' -k 3 -nr \
  | head -n 1 \
  | cut -d ';' -f2)"

cat "planets.txt" \
  | tail -n +2 \
  | sort -t ';' -k 3 -n \
  | awk -v TYPE="$type" -F ';' '{if ($2 == TYPE) {print $0}}' \
  | head -n +1 \
  | awk -F ';' '{printf "%s\t%s\n", $1, $4}'
