#!/bin/bash

find ~ -type d -print0 2> /dev/null | xargs -0 chmod -R +755
