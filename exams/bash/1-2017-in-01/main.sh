#!/bin/bash

user="$(whoami)"
find / -user "${user}" 2> /dev/null 
