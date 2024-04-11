#!/bin/bash

load_data () {
  # you should use:
  # cat "/etc/passwd"
  # but to test with example data
  cat "example-passwd"
}

data="$(while read line; do
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
done < <(load_data | awk -F ':' '{printf "%s:%s:%s\n", $1, $5, $6}'))"

while read line; do
  echo "$line"
  t_change="$(echo "$line" \
    | cut -d ' ' -f4 \
    | xargs stat -c "%Z")"
  if [[ "${t_change}" -ge 1709062214 && "${t_change}" -le 1709062235 ]]; then
    echo "${line}" | awk '{printf "%s\t%s\n", $1, $2}'
  fi
done < <(echo "${data}")
