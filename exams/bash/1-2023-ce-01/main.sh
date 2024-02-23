#!/bin/bash

find ~ -type f -user "$(whoami)" -regex ".*/*.blend[0-9]+" 
