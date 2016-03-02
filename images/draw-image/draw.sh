#!/bin/bash

if [ $# -lt 2 ]; then
  echo "Usage: $(basename $0) DRAWPY WORKDIR [LISTFILE]"
  exit 1
fi

DRAWPY="$1"
WORKDIR="$2"

exec 6<&0
if [ $# -gt 2 ]; then
  exec < "$3"
fi

while read line; do
  parts=( $line )
  img=${parts[0]##*/}
  python "$DRAWPY" "${parts[0]}" "$WORKDIR/$img" -s "${parts[1]}"
done

exec 0<&6 6<&-
