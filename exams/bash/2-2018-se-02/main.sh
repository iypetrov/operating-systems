#!/bin/bash

if [[ $# -ne 2 ]]; then
  echo "proovide 2 args"
  exit 99
fi

if ! [[ -f "$1" ]]; then
  echo "1 ars should be a file"
  exit 99
fi

if ! [[ -d "$2" ]]; then
  echo "2 ars should be a dir"
  exit 99
fi

el_dir=$(find "$2" | wc -l)
if [[ $el_dir -ne 1 ]]; then
  echo "2 ars should be an empty dir"
  exit 99
fi

SRC="$1"
TRG="$2"
touch "dict.txt"
> "dict.txt"

id=0
data="$(while read line; do
  name="$(echo "$line" \
    | sed -E 's/\(.*\)//g' \
    | cut -d ':' -f1 \
    | sed 's/[ \t]\+/ /g' \
    | sed -E 's/^[ \t]\+//g' \
    | sed -E 's/[ \t]+$//g')"
  echo "$name;$id" >> "dict.txt"
  echo "$name;$id"
  id=$((id + 1))
done < <(cat "$SRC"))"

while read line; do
  name="$(echo "$line" | cut -d ';' -f1)"
  id="$(echo "$line" | cut -d ';' -f2)"

  fname="$(echo "$name" | cut -d ' ' -f1)"
  lname="$(echo "$name" | cut -d ' ' -f2)"

  touch "${id}.txt"
  info="$(cat "$SRC" \
    | sed -E 's/\(.*\)//g' \
    | grep -E "^[ \t]*${fname}[ \t]+${lname}.*$" \
    | cut -d ':' -f2)"

  echo "$info" > "${id}.txt" 
done < <(echo "$data")
