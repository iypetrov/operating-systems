#!/bin/bash

# ! don't run this script, it will harm your machine

# a
find . -type f -empty -print0 2> /dev/null | xargs -0 rm

# b
find ~ -type d -print0 2> /dev/null | xargs -0 rm -rf

while read file; do
  size="$(stat -c '%s' "${file}")"
  echo "${file} ${size}"
done < <(find ~ -type f 2> /dev/null) \
  | sort -t ' ' -k 2 -nr \
  | head -n 5 \
  | xargs rm -rf
