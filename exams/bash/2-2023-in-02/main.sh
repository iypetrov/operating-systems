#!/bin/bash

if [[ "$#" -ne 1 ]]; then
  echo "provide 1 arg" >2&
  exit 99
fi

if ! [[ -d "$1" ]]; then
  echo "1 arg should be a dir" >2& 
  exit 99
fi

SRC="$1"

file_cnt="$(while read file; do
  sha256sum "${file}" \
    | sed 's/ \+/ /g' \
    | cut -d ' ' -f1
done <  <(find "$SRC" -type f) \
    | sort \
    | uniq)"

while read chk; do
    all_file_cnt="$(while read file; do
        sha256sum "${file}"
    done <  <(find "$SRC" -type f) \
        | sed 's/ \+/ /g' \
        | grep -x -E "${chk}.*" \
        | cut -d ' ' -f2)"

    info="$(while read line; do
        inode="$(echo "${line}" | xargs stat -c "%i")"
        echo "${line}" "${inode}"
    done < <(echo "$all_file_cnt"))"

    info_size="$(echo "${info}" | wc -l)"
    info_un_size="$(echo "${info}" \
        | cut -d ' ' -f2 \
        | sort \
        | uniq \
        | wc -l)"

    if [[ "${info_size}" -ne "${info_un_size}" ]]; then
        if [[ "${info_un_size}" -eq "1" ]]; then
            # hard links
            echo "${info}" \
                | cut -d ' ' -f1 \
                | head -n 1 
        else
            # hard links + copy
            copy_inodes="$(echo "${info}" \
                | cut -d ' ' -f2 \
                | sort \
                | uniq -c \
                | sed 's/^[[:space:]]*//g' \
                | awk '{ if($1 == '1') {printf "%s\n", $0} }' \
                | cut -d ' ' -f2)"

            while read line; do
                ind="$(echo "${line}" | cut -d ' ' -f2)"
                if echo "${copy_inodes}" | grep -qs "${ind}"; then
                    echo "${line}" | cut -d ' ' -f1
                fi
            done < <(echo "${info}")
            
            hard_links="$(while read line; do
                ind="$(echo "${line}" | cut -d ' ' -f2)"
                if ! echo "${copy_inodes}" | grep -qs "${ind}"; then
                    echo "${line}" | cut -d ' ' -f1
                fi
            done < <(echo "${info}"))"

            echo "${hard_links}" \
                | cut -d ' ' -f1 \
                | head -n 1 
        fi
    else
        # copy
        echo "${info}" \
            | cut -d ' ' -f1 \
            | tail -n +2 
    fi
done < <(echo "${file_cnt}") 
