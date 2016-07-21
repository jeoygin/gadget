#!/usr/bin/env bash

PKG_DIR="/Users/jeoygin/.stack/indices/Hackage/packages"
 
stack list-dependencies | while read dep
do
  _pkg=$( echo "$dep" | awk '{print $1}' )
  _version=$( echo "$dep" | awk '{print $2}' )
  _filename="${_pkg}-${_version}.tar.gz"
  _filepath="${PKG_DIR}/${_pkg}/${_version}/${_filename}"
  if [ ! -e "${_filepath}" ]; then
    echo -n "Downloading $_filename..."
    mkdir -p "$( dirname "${_filepath}" )"
    wget -O "${_filepath}" "http://hackage.fpcomplete.com/package/${_filename}" > /dev/null 2>&1
    if [ $? -eq 0 ]; then
      echo "DONE"
    else
      echo "FAIL"
    fi
  fi
done
