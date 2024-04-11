#!/bin/bash

load_data () {
  # you should use:
  # cat "/etc/passwd"
  # but to test with example data
  cat "example-passwd"
}

while read line; do
    fn="$(echo "${line}" \
      | cut -d ':' -f1 \
      | tr -d 's' )"
    name="$(echo "${line}" \
      | cut -d ':' -f2 \
      | cut -d ',' -f1)"
    path="$(echo "${line}" | cut -d ':' -f3)"
    spec="$(echo "${path}" | cut -d '/' -f3)"

    if [[ "${spec}" = "SI" ]]; then
      echo "${fn} ${name} ${path}"
    fi
done < <(load_data \
  | awk -F ':' '{printf "%s:%s:%s\n", $1, $5, $6}') \
  | sort -t ' ' -k 1 -n
