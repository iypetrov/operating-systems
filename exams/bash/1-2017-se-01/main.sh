#!/bin/bash

find . -type f -printf "%p %n\n" | sort -t ' ' -k 2 -nr | head -n 5 | cut -d ' ' -f1
