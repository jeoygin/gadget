#!/usr/bin/env python

import cv2
import numpy as np
import os, sys, getopt

def main(argv):
    for line in sys.stdin:
        paths = line.strip().split(' ')
        dstdir = os.path.dirname(paths[1])
        if not os.path.isdir(dstdir):
            os.makedirs(dstdir)

        imagefile = paths[0]
        outfile = paths[1]
        img = cv2.imread(imagefile)
        dst = (255 - img)
        cv2.imwrite(outfile, dst)

if __name__ == "__main__":
    main(sys.argv[1:])
