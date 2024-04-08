#!/bin/bash

load_info_raid() {
  cat "a.txt"
}

if [[ "$(whoami)" != "root" ]]; then
  echo "you dont have perm to run this script" >&2
  exit 99
fi

if [[ "$#" -gt 1 ]]; then
  echo "provide 0 or 1 args" >&2
  exit 99
fi

SLOTS="0"
if [[ -n "$(echo "$CTRLSLOTS")" ]]; then
  SLOTS="$(echo "$C")"
fi

PREFIX="SSA"

if [[ "$#" -eq 0 ]]; then
  while read slot; do
    data="$(load_info_raid "$slot")"
    while IFS= read -r line; do
      if echo "$line" | grep -qs -E '^[ ]{0}[^ ].*$'; then 
        n_slot="$(echo "$line" | cut -d ' ' -f6)"
        model="$(echo "$line" | cut -d ' ' -f3)"
      fi

      if echo "$line" | grep  -qs -E '^[ ]{2}[^ ].*$'; then 
        if echo "$line" | grep -F "  Unassigned"; then
          mass="UN"
        fi
        mass="$(echo "$line" \
          | sed -E 's/^[ \t]+//g' \
          | cut -d " " -f2 \
          | tr -d '\n')"
      fi

      if echo "$line" | grep -qs -E '^[ ]{4}[^ ].*$'; then 
        name="$(echo "$line" \
          | sed -E 's/^[ \t]+//g' \
          | cut -d " " -f3 \
          | tr -d ':')" 
      fi

      if echo "$line" | grep -qs -E '^[ ]{6}[^ ].*$'; then 
        temp="$(echo "$line" \
          | sed -E 's/^[ \t]+//g' \
          | grep -E '^Current Temperature \(C\):' \
          | rev \
          | cut -d ' ' -f1 \
          | rev)"
      fi

      if [[ -n "$PREFIX" && -n "$n_slot" && -n "$model" && -n "$mass" && -n "$name" && -n "$temp" ]]; then
        echo "${PREFIX}${n_slot}${model}${mass}${name}.value ${temp}"
      fi
    done < <(echo "$data" | grep -v -E '^$')
  done < <(echo "$SLOTS" | tr ' ' '\n')
elif [[ "$1" = "autoconf" ]]; then
  echo "yes"
elif [[ "$1" = "config" ]]; then
  echo "graph_title $PREFIX drive temperatures"
  echo "graph_vlabel Celsius" 
  echo "graph_category sensors"
  while read slot; do
    data="$(load_info_raid "$slot")"
    while IFS= read -r line; do
      if echo "$line" | grep -qs -E '^[ ]{0}[^ ].*$'; then 
        n_slot="$(echo "$line" | cut -d ' ' -f6)"
        model="$(echo "$line" | cut -d ' ' -f3)"
      fi

      if echo "$line" | grep  -qs -E '^[ ]{2}[^ ].*$'; then 
        if echo "$line" | grep -F "  Unassigned"; then
          mass="UN"
        fi
        mass="$(echo "$line" \
          | sed -E 's/^[ \t]+//g' \
          | cut -d " " -f2 \
          | tr -d '\n')"
      fi

      if echo "$line" | grep -qs -E '^[ ]{4}[^ ].*$'; then 
        name="$(echo "$line" \
          | sed -E 's/^[ \t]+//g' \
          | cut -d " " -f3 \
          | tr -d ':')" 
        nofname="$(echo "$line" \
          | sed -E 's/^[ \t]+//g' \
          | cut -d " " -f3)" 
      fi

      if echo "$line" | grep -qs -E '^[ ]{6}[^ ].*$'; then 
        temp="$(echo "$line" \
          | sed -E 's/^[ \t]+//g' \
          | grep -E '^Current Temperature \(C\):' \
          | rev \
          | cut -d ' ' -f1 \
          | rev)"
      fi

      if [[ -n "$PREFIX" && -n "$n_slot" && -n "$model" && -n "$mass" && -n "$name" && -n "$temp" ]]; then
        echo "${PREFIX}${n_slot}${model}${mass}${name}.label ${PREFIX}${n_slot} ${model} ${mass} ${nofname}"
        echo "${PREFIX}${n_slot}${model}${mass}${name}.type GAUGE"
      fi
    done < <(echo "$data" | grep -v -E '^$')
  done < <(echo "$SLOTS" | tr ' ' '\n')
fi
