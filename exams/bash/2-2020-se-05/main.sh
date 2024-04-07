#!/bin/bash

if [[ $# -ne 3 ]]; then
  echo "provide 3 args" >&2
  exit 99
fi

if ! [[ -f "$1" ]]; then
  echo "1 arg should be a file" >&2
  exit 99
fi

if ! [[ -d "$3" ]]; then
  echo "3 arg should be a dir" >&2
  exit 99
fi

USRS="$1"     
TRG="$2"
SRC="$3"

touch "$TRG"

while read file; do
  fdata="$(cat "$file" \
    | cut -d '#' -f1 \
    | sed 's/ \+//g' \
    | sed 's/{no-production};//g' \
    | sed 's/{volatile};//g' \
    | sed 's/{run-all;};//g' \
    | awk '{ if ($0 != "") {print $0} }')" 
  cfdata="$(echo "$fdate" | wc -l)"
  if [[ "$cfdata" -ne 1 ]]; then
    echo "Errors in file $file"
    cat "$file" | awk '{cnt++; printf "%d:%s\n", cnt, $0}' \
        | grep -v -E '^[0-9]+:#.*$' \
        | grep -v -E '^[0-9]+:\{ no-production \};$' \
        | grep -v -E '^[0-9]+:\{ volatile \};$' \
        | grep -v -E '^[0-9]+:\{ run-all; \};$' \
        | grep -v -E '^[0-9]+:[ \t]*' \
        | awk 'printf "Line %s\n", $0'
  else
    cat "$file" >> "$TRG"
    name="$(basename "$file" | rev | cut -d '.' -f2- | rev)"
    if ! cat "$USRS" | cut -d ':' -f1 | grep -qs "$name"; then
        psw="$(pwgen 16 1 | md5sum | cut -d ' ' -f1 | head -c 16)"
        echo "${name}:${psw}" >> "$USRS"
    fi
  fi
done < <(find "$SRC" -type f -name '*.cfg')
