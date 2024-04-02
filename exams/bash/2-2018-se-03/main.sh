#!/bin/bash

if [[ $# -ne 2 ]]; then
  echo "provide 2 arsg"
  exit 99
fi

if ! [[ -f "$1" ]]; then
  echo "1 arg should be a file"
  exit 99
fi

if ! echo "$1" | grep -qs -E '^.*\.csv$'; then
  echo "1 arg should be a csv file"
  exit 99
fi

if ! echo "$2" | grep -qs -E '^.*\.csv$'; then
  echo "2 arg should be a end wiht .csv"
  exit 99
fi

CSV="$1"
TRG="$2"

keys="$(cat "$CSV" \
  | cut -d ',' -f2- \
  | sort \
  | uniq)"

touch "$TRG"
while read key; do
  cat "$CSV" \
    | grep -E "^[0-9]+,[ \t]*${key}$" \
    | sort -t ',' -k 1 -n \
    | head -n 1 >> "$TRG"
done < <(echo "$keys")

