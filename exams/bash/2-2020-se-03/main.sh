#!/bin/bash

STG="$(realpath $1)"
PKG="$(realpath $2)"

if ! [[ -d "$STG/packages"  ]]; then
    mkdir -p "$STG/packages" 
fi

pkg_ver="$(cat "$PKG/version")"
pkg_arch="$(tar -cJvf tmp.tar.xz "$PKG/tree" 2> /dev/null)"
chk="$(sha256sum tmp.tar.xz | cut -d ' ' -f1)"
mv tmp.tar.xz $chk.tar.xz

pkg_name="$(basename $STG)-$pkg_ver"
pkg_wt_ver="$(cat "$STG/db" | cut -d '-' -f1)"

if [[ -z "${pkg_wt_ver}" ]]; then
    # create
    if [[ -n "$chk" ]]; then
        mv $chk.tar.xz "$STG/packages/$chk.tar.xz"
        echo "$STG/packages/$chk.tar.xz"
        echo "$pkg_name $chk" >> "$STG/db"
    fi
else
    # update 
    old_ver="$(cat "$STG/db" \
        | grep -E "^${pkg_wt_ver}-.*$" \
        | cut -d ' ' -f1 \
        | cut -d '-' -f2-)"
   
    if [[ "$pkg_ver" = "$old_ver" ]]; then 
        rm $chk.tar.xz
        exit 0
    fi

    old_chk="$(cat "$STG/db" \
        | grep -E "^${pkg_wt_ver}-.*$" \
        | cut -d ' ' -f2)"

    line="$(cat "$STG/db" \
        | grep -cE "^${pkg_wt_ver}-.*$" \
        | cut -d ':' -f1)"
    
    rm "$STG/packages/${old_chk}.tar.xz"
    sed -i "${line}c $pkg_name $chk" "$STG/db"
    mv $chk.tar.xz "$STG/packages/$chk.tar.xz"
fi

