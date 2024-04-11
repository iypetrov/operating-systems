#!/bin/bash

load_data () {
  # you should use:
  # cat "/etc/passwd"
  # but to test with example data
  cat "example-passwd"
}

data="$(load_data \
  | awk -F ':' '{printf("%s:%s:%s\n", $1, $5, $6)}' \
  | tr '/' '|' \
  | awk -F ':' '{ if (match($3, "/|home|Inf|/")) { printf("%s\n", $0) } }' \
  | awk -F ':' '{printf("%s %s\n", $1, $2)}' \
  | tr ',' ' ' \
  | awk '{printf("%s %s %s\n", $1, $2, $3)}')"

fns="$(echo "$data" \
  | grep -E "a$" \
  | cut -d ' ' -f1)"

echo "$fns" \
  | cut -c 3-4 \
  | sort -rn \
  | uniq -c \
  | head -n 1 
