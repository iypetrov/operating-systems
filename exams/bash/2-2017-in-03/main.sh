#!/bin/bash

last_mod_file() {
    if [[ $# -ne 1 ]]; then
        echo "provide 1 arg"
        exit 99
    fi

    if [[ -d "$1" ]]; then
        find "$1" -maxdepth 1 -type f | xargs stat -c "%n %U %Y" | sort -t ' ' -k 3 -nr | head -n +1
    fi

}

last_mod_files_all_users () {
    while read line; do 
        last_mod_file "$line" 2> /dev/null
    done < <(cat /etc/passwd | cut -d ':' -f6)
}

last_mod_files_all_users | sort -t ' ' -k 3 -nr | head -n +1 | cut -d ' ' -f2
