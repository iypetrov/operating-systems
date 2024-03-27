#!/bin/bash

if [[ $# -lt 2 ]];then
    echo "provide at least 2 args"
    exit 99
fi

if ! [[ "$1" =~ ^[0-9]+$ ]]; then
    echo "1 arg should be a number"
    exit 99
fi

LIM=$1

cmd="$(echo "$@" | cut -d ' ' -f2-)"

cnt=0
start="$(date +'%s.%N')"
while true; do
    end="$(date +'%s.%N')"
    diff="$(echo "$end $start" | awk '{printf "%f", $1 - $2}')"
    "$cmd" &> /dev/null
    if [[ $(echo "$diff $LIM" | awk '{if($1 >= $2) {print 0} else {print 1}}') -eq 0 ]]; then
        break;
    fi
    cnt=$((cnt + 1))
done

diff="$(echo "$diff" | awk '{printf "%.2f", $1}')"
echo "Executed command was $cmd"
echo "It took $diff seconds"
echo "Command was executed $cnt times"

avg="$(echo "$cnt $LIM" | awk '{printf "%.2f", $1 / $2}')"
echo "Avg runtime is $avg executions per second"
