#!/bin/bash

lt="$(cat "input.txt" | tail -n +2 | grep "|Success|" | cut -d '|' -f2 | sort | uniq -c | head -n +1 | sed 's/^[[:space:]]*//' | cut -d ' ' -f2)"
cat "input.txt" | tail -n +2 | grep "|$lt|" | sort -t '|' -k 1 -nr | head -n +1 | awk -F '|' '{printf "%s:%s\n", $3, $4}' 
