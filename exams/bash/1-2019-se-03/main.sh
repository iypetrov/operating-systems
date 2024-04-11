#!/bin/bash

get_depth_file() {
    echo "$1" \
      | tr '/' '\n' \
      | wc -l \
      | awk '{printf "%d\n", $0 - 1}'
}

vdir="$(cat "/etc/passwd" \
  | grep -E '^velin:.*$' \
  | cut -d ':' -f6)"
echo "vdir -> ${vdir}"

inode_last="$(find "${vdir}"  -type f -print0 \
  | xargs -0 stat -c "%i %n %Z" \
  | sort -t ' ' -k 3 -nr \
  | head -n +1 \
  | cut -d ' ' -f1)"

while read line; do
    depth="$(get_depth_file ${line})"
    echo "${line} ${depth}"
done < <(find   -type f -print0 \
  | xargs -0 stat -c "%i %n" \
  | awk -v LST="$inode_last" '{ if($1 == LST) {printf "%s\n", $2}}') \
  | sort -t ' ' -k 2 -n \
  | head -n +1 \
  | cut -d ' ' -f1
