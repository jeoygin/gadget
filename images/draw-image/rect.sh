#!/bin/bash

if [ $# -lt 1 ]; then
  echo "Usage: $(basename $0) WORKDIR [LISTFILE]"
  exit 1
fi

WORK_DIR="$1"
test -d "$WORK_DIR" || mkdir -p "$WORK_DIR"

exec 6<&0
if [ $# -gt 1 ]; then
  exec < "$2"
fi

while read line; do
  parts=( $line )
  id=${parts[0]}
  shapefile="$WORK_DIR/$id.shape"
  test -f "$shapefile" && rm -f "$shapefile"
  for ((i=1; i<${#parts[@]}; i++))
  do
    awk "{print \"rect\",\$1,\$2,\$3,\$4,$i}" "${parts[i]}" >> "$shapefile"  
  done
done

exec 0<&6 6<&-
