#!/bin/bash

if [ -z "$1" ]; then
	echo Usage: "$0" cfgfile
	exit 2
fi

cfg="$1"

if ! [ -f "$cfg" ]; then
	echo File "$cfg" is missing
	exit 1
fi

regex='^(#.*|\{[^{}]*\};|)$'

if grep -v -q -E "$regex" "$cfg"; then
	grep -v -n -E "$regex" "$cfg"|sed 's/^/Line /'
	exit 1
fi

exit 0
