#!/bin/bash

validate_zone_file () {
    data="$(echo "$1" \
        | cut -d ';' -f1 \
        | tr '\n' ' ' \
        | tr -d '(' \
        | tr -d ')' \
        | sed 's/^[ \t]*//g' \
        | sed 's/[ \t]\+/ /g')"

    data_fields_num=$(echo "$data" \
        | tr ' ' '\n' \
        | wc -l)
    
    #if [[ $data_fields_num -lt 5 ]];then
    #    echo "zone file is not in correct format"
    #    exit 99
    #fi

    key="$(echo "$data" | cut -d ' ' -f1)"
    ttl="$(echo "$data" | cut -d ' ' -f2)"
    class="$(echo "$data" | cut -d ' ' -f3)"
    type="$(echo "$data" | cut -d ' ' -f4)"
    rdata="$(echo "$data" | cut -d ' ' -f5-)"
}

TRG="$1"

#validate_zone_file "$TRG"
data_fields_num=$(cat "$TRG" | wc -l)
echo "$data_fields_num"
if [[ $data_fields_num -eq 1 ]];then
    new_serial=""
    curr_date="$(date +'%Y%m%d')"
    serial_date="$(cat "$TRG" \
        | rev \
        | cut -d ' ' -f6 \
        | rev \
        | head -c 8)"

    if [[ "$curr_date" != "$serial_date" ]]; then
        new_serial_date="${curr_date}00"
    else
        serial_num="$(cat "$TRG" \
            | rev \
            | cut -d ' ' -f6 \
            | rev \
            | tail -c 2)"
        fchar="${serial_num:0:1}"
        if [[ "$fchar" =~ "0" ]]; then
            serial_num="${serial_num:1:1}"
        fi
        serial_num="$(echo "$serial_num" \
            | awk '{printf "%d\n", $1 + 1}')"
        if [[ ${#serial_num} -eq 1 ]]; then
            serial_num="0$serial_num"
        fi
        new_serial_date="$curr_date$serial_num"
    fi

    res=$(cat "$TRG" \
        | tr ' ' '\n' \
        | wc -l)
    res=$((res - 4))
    
    cat "$TRG" \
        | tr ' ' '\n' \
        | awk -v RES="$RES" -v SER="$new_serial_date" 'BEGIN{cnt=0} {cnt++; if(cnt==RES) { printf "%s ", SER } else { printf "%s ", $1 } END{ print } }' > "$TRG"

    echo "$new_serial_date"
else
    new_serial=""
    curr_date="$(date +'%Y%m%d')"
    serial_date="$(cat "$TRG" \
        | sed -n '2p' \
        | cut -d ';' -f1 \
        | tr '\n' ' ' \
        | sed 's/^[ \t]*//g' \
        | sed 's/[ \t]\+/ /g' \
        | head -c 8)"

    if [[ "$curr_date" != "$serial_date" ]]; then
        new_serial_date="${curr_date}00"
    else
        serial_num="$(cat "$TRG" \
            | cut -d ' ' -f7 \
            | tail -c 2)"
        fchar="${serial_num:0:1}"
        if [[ "$fchar" =~ "0" ]]; then
            serial_num="${serial_num:1:1}"
        fi
        serial_num="$(echo "$serial_num" \
            | awk '{printf "%d\n", $1 + 1}')"
        if [[ ${#serial_num} -eq 1 ]]; then
            serial_num="0$serial_num"
        fi
        new_serial_date="$curr_date$serial_num"
    fi
    
    echo "$new_serial_date"
fi

