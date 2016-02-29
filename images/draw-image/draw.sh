#!/bin/bash

if [ $# -lt 4 ]; then
  echo "Usage: $(basename $0) drawpy imglist srcdir dstdir"
  exit 1
fi

drawpy="$1"
imglist="$2"
src="$3"
dst="$4"

for img in `cat $imglist`
do
  python "$drawpy" "$src/$img" "$dst/$img" "$dst/$img.draw"
done
