#!/bin/bash

get_mem_usage_file () {
    df "$1" \
        | tail -n +2 \
        | sed 's/[ \t]\+/ /g' \
        | cut -d ' ' -f5 \
        | rev \
        | tail -c +2 \
        | rev
}

get_stat_backup_obj () {
    hostname="$1"
    area="$2"
    backup="$(realpath "$3")"

    year=0
    while read file; do
        if [[ -h "$file" ]]; then 
            file="$(readlink "$file" | xargs realpath)"
        fi

        file="$(basename "$file")"
        
        h="$(echo "$file" | cut -d '-' -f1)"
        a="$(echo "$file" | cut -d '-' -f2)"

        if [[ "$hostname" = "$h" ]]; then
            if [[ "$area" = "$a" ]]; then
                year=$((year + 1))
            fi
        fi
    done < <(find "$backup/1" -type f,l)

    month=0
    while read file; do
        if [[ -h "$file" ]]; then 
            file="$(readlink "$file" | xargs realpath)"
        fi
        
        file="$(basename "$file")"

        h="$(echo "$file" | cut -d '-' -f1)"
        a="$(echo "$file" | cut -d '-' -f2)"

        if [[ "$hostname" = "$h" ]]; then
            if [[ "$area" = "$a" ]]; then
                month=$((month + 1))
            fi
        fi
    done < <(find "$backup/2" -type f,l)

    week=0
    while read file; do
        if [[ -h "$file" ]]; then 
            file="$(readlink "$file" | xargs realpath)"
        fi
        
        file="$(basename "$file")"

        h="$(echo "$file" | cut -d '-' -f1)"
        a="$(echo "$file" | cut -d '-' -f2)"

        if [[ "$hostname" = "$h" ]]; then
            if [[ "$area" = "$a" ]]; then
                week=$((week + 1))
            fi
        fi
    done < <(find "$backup/3" -type f,l)

    day=0
    while read file; do
        if [[ -h "$file" ]]; then 
            file="$(readlink "$file" | xargs realpath)"
        fi
        
        file="$(basename "$file")"

        h="$(echo "$file" | cut -d '-' -f1)"
        a="$(echo "$file" | cut -d '-' -f2)"

        if [[ "$hostname" = "$h" ]]; then
            if [[ "$area" = "$a" ]]; then
                day=$((day + 1))
            fi
        fi
    done < <(find "$backup/4" -type f,l)

    echo "$year $month $week $day"
}


clean_up_backups () {
    hostname="$1"
    area="$2"
    src="$(realpath $3)"
    trg=$4
    
    files="$(while read file; do
        if [[ -h "$file" ]]; then 
            file="$(readlink "$file" | xargs realpath)"
        fi

        if basename "$file" | grep -qsE '^[a-z]+-[a-z]+-[0-9]{8}\.tar\.xz$'; then
            mem="$(get_mem_usage_file "$file")"
            echo "$file $mem" 
        fi
    done < <(find "$SRC" -type f,l))"

    files_len=$(echo "$files" | wc -l)

    if [[ $files_len -gt $trg ]]; then
        diff=$((files_len - trg))
        del_backups="$(echo $files \
            | sort -t ' ' -k 2 -nr \
            | head -n $diff \
            | cut -d ' ' -f1)"
        while read b; do
            b="$(realpath b)"
            rm "$b"
        done < <(echo "$del_backups")
    fi
}

if [[ $# -ne 2 ]]; then
    echo "provide 2 args"
    exit 99
fi

if ! [[ -d "$1" ]]; then
    echo "1 arg should be a dir"
    exit 99
fi

if ! [[ $2 =~ ^[0-9]+$ ]]; then
    echo "2 args should be a number"
    exit 99
fi

SRC="$(realpath "$1")"
LIM="$2"

pairs="$(while read file; do
    if [[ -h "$file" ]]; then 
        file="$(readlink "$file" | xargs realpath)"
    fi

    if basename "$file" | grep -qsE '^[a-z]+-[a-z]+-[0-9]{8}\.tar\.xz$'; then
        echo "$file" \
            | xargs basename \
            | cut -d '-' -f1-2 \
            | tr '-' ' '
    fi
done < <(find "$SRC" -type f,l) \
    | sort \
    | uniq)"

while read pair; do
    hostname="$(echo "$pair" | cut -d ' ' -f1)"
    area="$(echo "$pair" | cut -d ' ' -f2)"
    res="$(get_stat_backup_obj "$hostname" "$area" "$SRC")"
    year=$(echo "$res" | cut -d ' ' -f1)
    month=$(echo "$res" | cut -d ' ' -f2)
    week=$(echo "$res" | cut -d ' ' -f3)
    day=$(echo "$res" | cut -d ' ' -f4)

    echo "$hostname-$area ---> $year $month $week $day"

    if [[ $year -lt 1 ]]; then
        clean_up_backups "$hostname" "$area" "$SRC/1" 1
    fi

    if [[ $month -lt 2 ]]; then
        clean_up_backups "$hostname" "$area" "$SRC/2" 2
    fi

    if [[ $week -lt 3 ]]; then
        clean_up_backups "$hostname" "$area" "$SRC/3" 3
    fi

    if [[ $day -lt 4 ]]; then
        clean_up_backups "$hostname" "$area" "$SRC/4" 4
    fi
done < <(echo "$pairs")
