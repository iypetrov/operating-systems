#!/bin/bash

if [[ "$(whoami)" != "grid" && "$(whoami)" != "oracle" ]]; then
    echo "you don't have the permissions to run this script"
    exit 99
fi

if [[ "$(whoami)" = "grid" ]]; then
    ROLE="SYSASM"
elif [[ "$(whoami)" = "oracle" ]]; then
    ROLE="SYSDBA"
fi

if [[ "$#" -ne 1 ]]; then
    echo "provide 1 arg"
    exit 99
fi

if ! [[ "$1" =~ [0-9]+ ]]; then
    echo "1 arg should be an int"
    exit 99
fi

DAYS="$1"

if [[ -z echo "${ORACLE_BASE}" ]];then
    echo "ORACLE_BASE doesn't exist"
    exit 99
else
    ORACLE_BASE="${ORACLE_BASE}"
fi

if [[ -z echo "${ORACLE_HOME}" ]];then
    echo "ORACLE_HOME doesn't exist"
    exit 99
else
    ORACLE_HOME="${ORACLE_HOME}"
fi

if [[ -z echo "${ORACLE_SID}" ]];then
    echo "ORACLE_SID doesn't exist"
    exit 99
else
    ORACLE_SID="${ORACLE_SID}"
fi

while IFS= read -rd $'\0' file; do
    echo "${file}"
done < <(find "${ORACLE_HOME}" -executable -name 'sqlplus' -print0)
