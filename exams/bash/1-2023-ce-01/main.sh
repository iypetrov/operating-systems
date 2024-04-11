#!/bin/bash

# we should use ~, but to test will use data dir
find data -type f -user "$(whoami)" -regex ".*/*.blend[0-9]+" 
