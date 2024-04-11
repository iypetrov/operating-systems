#!/bin/bash

SRC="var/logs/my-logs"

while read file; do
  cat "${file}" | tr ' ' '\n'
done< <(find "$SRC" -type f \
  | rev \
  | cut -d '/' -f1 \
  | rev \
  | grep -E '^[a-zA-Z0-9_]+_[0-9]+\.log$' \
  | awk -v PREFIX="${SRC}" '{ printf "%s/%s\n", PREFIX, $0 }') \
  | grep -i -E 'error' \
  | wc -l
