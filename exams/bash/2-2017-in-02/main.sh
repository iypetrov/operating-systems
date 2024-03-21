#!/bin/bash

number_ps () {
    ps -e -o user | tail -n +2 | sort | uniq -c | grep "$1" | sed 's/^[[:space:]]*//g' | cut -d ' ' -f1
}

get_avg_ps_time() {
    num=$(ps -e -o time | tail -n +2 | sort | uniq | wc -l)
    ttime=$(ps -e -o time | tail -n +2 | tr ':' '\n' | awk 'BEGIN{c=0} {c+=$0} END{printf "%d\n", c}')
    echo $(($ttime / $num))
}

if [[ "$#" -ne 1 ]];then
    echo "provide 1 arg"
    exit 99
fi

if [[ $(id -u) -eq 0 ]]; then
    n_ps=$(number_ps "$1")

    while read line; do 
        curr_n_ps=$(number_ps "$line")
        if [[ $curr_n_ps -gt $n_ps ]]; then
            echo "$line"
        fi
    done < <(ps -e -o user | tail -n +2 | sort | uniq)

    avg=$(get_avg_ps_time)
    lim=$((2 * $avg))
    
    while read line; do 
        hours="$(echo "$line" | cut -d ' ' -f2 | cut -d ':' -f1)"
        secs="$(echo "$line" | cut -d ' ' -f2 | cut -d ':' -f2)"
        mins="$(echo "$line" | cut -d ' ' -f2 | cut -d ':' -f3)"
        if [[ $((hours + mins + secs)) -gt $lim ]];then
            pid="$(echo "$line" | cut -d ' ' -f1)"
            kill -9 $pid
        fi
    done < <(ps -e -o pid,time | tail -n +2)

else
    echo "run as root"
    exit 99
fi

