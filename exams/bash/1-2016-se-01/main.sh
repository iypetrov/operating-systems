#!/bin/bash

cat "input.txt" | grep -v [a-w] | grep [02468] | wc -l
