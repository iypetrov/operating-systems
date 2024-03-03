#!/bin/bash

if [[ ! $(id -u) -eq 0 ]]; then
    echo "run the command as root user"
    exit 99
fi

while read line; do
    path="$(echo "$line" | cut -d ':' -f6)"
    user_id="$(echo "$line" | cut -d ':' -f3)"
    group_id="$(echo "$line" | cut -d ':' -f4)"

    if [[ -z "$path" ]]; then
        echo "$line" | awk -F ':' '{printf "%s:%s\n", $3, $4}'
    else
        if [[ ! -d "$path" ]]; then
           cut -d ':' -f1 
           continue
        fi
        
        is_user=$(echo "$path" | xargs stat -c "%u")
        is_group=$(id -G | tr ' ' '\n' | grep -qs "$group_id")
        if [[ "$is_user" != "$user_id" || is_group -eq 1 ]]; then
            cut -d ":" -f1            
        fi
    fi
done < <(cat /etc/passwd)
