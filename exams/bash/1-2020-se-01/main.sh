#!/bin/bash

find ~ -type f -perm 644 -exec chmod g+w {} \;
