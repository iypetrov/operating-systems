#!/bin/bash

if [[ $# -ne 1 ]]; then
  echo "provide 1 arg"
  exit 99
fi

if ! [[ -d "$1" ]]; then
  echo "1 arg should be a dir"
  exit 99
fi

SRC="$1"
USR="$(whoami)"

while read dir; do
  usr="$(basename "$dir")"

  if [[ "$USR" = "$usr" ]]; then
    TRG="$USR"
    PATH="$dir"
    break
  fi
done < <(find "$SRC" -maxdepth 2 -mindepth 2 -type d)

if [[ -z "$TRG" ]]; then
  echo "you have no chats"
  exit 99
fi

while read fr_dir; do
  fr="$(basename "$fr_dir")"
  cnt=0
  while read file; do
    if basename "$file" | grep -qs -E '^[0-9]{4}-[0-9]{2}-[0-9]{2}-[0-9]{2}-[0-9]{2}-[0-9]{2}\.txt$'; then
      i=$(cat "$file" | wc -l)
      cnt=$((cnt + i))
    fi
  done < <(find "$fr_dir" -type f)
  echo "$fr $cnt"
done < <(find "$PATH" -maxdepth 1 -mindepth 1 -type d) \
  | sort -t ' ' -k2 nr \
  | head




