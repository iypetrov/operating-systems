#!/bin/bash

cosmodrum="$(cat "spacex.txt" \
  | tail -n +2 \
  | awk -F '|' '{ {printf "%s %s\n", $3, $0} }' \
  | grep -E '^Success' \
  | cut -d ' ' -f2 \
  | cut -d '|' -f2 \
  | sort \
  | uniq -c \
  | head -n +1 \
  | sed 's/^[[:space:]]*//' \
  | cut -d ' ' -f2)"

cat "spacex.txt" \
  | tail -n +2 \
  | awk -F '|' '{ {printf "%s %s\n", $2, $0} }' \
  | grep -E "^${cosmodrum}" \
  | sort -t '|' -k 1 -nr \
  | head -n +1 \
  | awk -F '|' '{printf "%s:%s\n", $3, $4}' 
