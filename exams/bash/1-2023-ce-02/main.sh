#!/bin/bash

while read line; do
    cat "./var/logs/my-logs/$line" | grep -c error
done< <(find ./var/logs/my-logs -type f -exec basename {} \; | awk '/[a-zA-Z0-9_]+_[0-9]+\.log$/{printf "%s\n", $0}') | awk 'BEGIN{ cnt=0 } {cnt+=$0} END{ print cnt }'

