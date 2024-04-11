#!/bin/bash

cat "philip-j-fry.txt" \
  | grep -E '[02468]' \
  | grep -v -E '[a-w]' \
  | wc -l
