#!/bin/bash

find / -printf "%p %u\n" | tail -n +2 | cut -d' ' -f1 2>/dev/null
