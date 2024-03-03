#!/bin/bash

find ~ -type f -user ipetrov -printf "%i %T@\n" | sort -t ' ' -k 2 -rn | head -n 1 | cut -d ' ' -f1
