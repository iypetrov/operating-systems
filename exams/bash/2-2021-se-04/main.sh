#!/bin/bash

if [[ "$(whoami)" != "ipetrov" && "$(whoami)" != "oracle" ]]; then
  echo "you don't have needed perms to run this script" >&2
  exit 99
fi

if [[ "$#" -ne 1 ]]; then
  echo "provide 1 arg"
  exit 99
fi

if ! [[ "$1" =~ [0-9]+ ]]; then
  echo "1 args should be a number"
  exit 99
fi

if [[ "$1" -lt 2 ]]; then 
  echo "1 arg should not be less than 2"
  exit 99
fi

DIAG_DEST="u01/app/$(whoami)"
while read file; do
  if echo "$file" | tr '/' ' ' | grep -qs "bin"; then
    ADRCI="$(realpath "$file")"
    break
  fi
done < <(find "$ORACLE_HOME" -type f -name 'adrci' 2> /dev/null)
"$("$ADRCI" exec "SET BASE $DIAG_DEST; for i in SHOW HOMES { SET HOMEAPTH i; PURGE -AGE $1 }"")"
