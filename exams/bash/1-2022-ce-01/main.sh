#!/bin/bash

find ~ -maxdepth 1 -type f -user "$(whoami)" -exec chmod g+w {} \; &>/dev/null
