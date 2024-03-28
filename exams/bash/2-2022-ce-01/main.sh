#!/bin/bash

base="$(cat base.csv)"
prefix="$(cat prefix.csv)"

NUM="$1"
PREFIX_SYMB="$2"
UNIT_SYMB="$3"

b_text_index="$(echo "$base" \
    | tail -n +2 \
    | cut -d ',' -f2 \
    | awk -v TRG="$UNIT_SYMB" '{cnt++; if ($1 == TRG) {printf "%d %s\n", cnt, $0}}' \
    | head -n 1 \
    | cut -d ' ' -f1)"
b_text_index=$((b_text_index + 1))
b_text="$(echo "$base" \
    | sed -n "${b_text_index}p" \
    | cut -d ',' -f1,3 \
    | tr ',' ' ')"
b_text="($b_text)"

p_text_index="$(echo "$prefix" \
    | tail -n +2 \
    | cut -d ',' -f2 \
    | awk -v TRG="$PREFIX_SYMB" '{cnt++; if ($1 == TRG) {printf "%d %s\n", cnt, $0}}' \
    | head -n 1 \
    | cut -d ' ' -f1)"
p_text_index=$((p_text_index + 1))
p_text="$(echo "$prefix" \
    | sed -n "${p_text_index}p" \
    | cut -d ',' -f3)"
sum=$(echo "$NUM $p_text" | awk '{sum=$1*$2; printf "%.1f\n", sum}')

echo "$sum s $b_text"
