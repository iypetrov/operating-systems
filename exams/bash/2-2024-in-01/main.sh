#!/bin/bash

load_config_file() {
    CONF_PATH="$ARKCONF"

    WHAT="$(cat "$CONF_PATH" | grep -E '^WHAT=' | cut -d '=' -f2 | tr -d '"')"
    WHERE="$(cat "$CONF_PATH" | grep -E '^WHERE=' | cut -d '=' -f2 | tr -d '"' | tr ' ' '\n')"
    WHO="$(cat "$CONF_PATH" | grep -E '^WHO=' | cut -d '=' -f2 | tr -d '"')"

    if [[ -z "$WHAT" ]]; then
        echo "config file is not valid, WHAT is missing"
        exit 99
    else
        LAST_SYMB="$(echo "$WHAT" | tail -c 2 | head -c 1)"
        if [[ "$LAST_SYMB" != "/" ]]; then
            echo "dir should end with /" 
            exit 99
        fi

        DIR="$(realpath "$WHAT")"
        if ! [[ -d "$DIR" ]]; then
            echo "provided arg is not a dir" 
            exit 99
        fi
    fi

    if [[ -z "$WHERE" ]]; then
        echo "config file is not valid, WHERE is missing"
        exit 99
    fi

    if [[ -z "$WHO" ]]; then
        echo "config file is not valid, WHO is missing"
        exit 99
    fi

    echo "$WHAT"
    echo "$WHERE"
    echo "$WHO"
}

WHAT="$(load_config_file | sed -n '1p')"
WHERE="$(load_config_file | sed -n '2p')"
WHO="$(load_config_file | sed -n '3p')"

if echo "$@" | tr ' ' '\n' | grep -qs "pull" && echo "$@" | tr ' ' '\n' | grep -qs "push"; then
    echo "use only pull or push"
    exit 99
fi

if echo "$@" | tr ' ' '\n' | grep -qs "pull"; then
    if echo "$@" | tr ' ' '\n' | grep -qs "-d"; then
        if [[ $# - eq 2 ]]; then
            while read server; do 
                rsync -a --delete "$WHO@$server:$WHAT" "$WHAT"  
            done < <(echo "$WHERE")
        fi

        if [[ $# - eq 3 ]]; then
            server="$echo "$WHERE" | grep -v -E '-d'"
            rsync -a --delete "$WHO@$server:$WHAT" "$WHAT" 
        fi
    else
        if [[ $# - eq 1 ]]; then
            while read server; do 
                rsync -a "$WHO@$server:$WHAT" "$WHAT"
            done < <(echo "$WHERE")
        fi

        if [[ $# - eq 2 ]]; then
            server="$echo "$WHERE" | grep -v -E '-d'"
            rsync -a "$WHO@$server:$WHAT" "$WHAT"
        fi
    fi
elif echo "$@" | tr ' ' '\n' | grep -qs "push"; then
    if echo "$@" | tr ' ' '\n' | grep -qs "-d"; then
        if [[ $# - eq 2 ]]; then
            while read server; do 
                rsync -a --delete "$WHAT" "$WHO@$server:$WHAT" 
            done < <(echo "$WHERE")
        fi

        if [[ $# - eq 3 ]]; then
            server="$echo "$WHERE" | grep -v -E '-d'"
            rsync -a --delete "$WHAT" "$WHO@$server:$WHAT" 
        fi
    else
        if [[ $# - eq 1 ]]; then
            while read server; do 
                rsync -a "$WHAT" "$WHO@$server:$WHAT" 
            done < <(echo "$WHERE")
        fi

        if [[ $# - eq 2 ]]; then
            server="$echo "$WHAT" "$WHERE" | grep -v -E '-d'"
            rsync -a "$WHO@$server:$WHAT" 
        fi
    fi
else
    echo "use pull or push"
    exit 99
fi 
