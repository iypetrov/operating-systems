#!/bin/bash

find . -type f -printf "%p %n\n" 2> /dev/null \
  | sort -t ' ' -k 2 -nr \
  | head -n 5 \
  | cut -d ' ' -f1
