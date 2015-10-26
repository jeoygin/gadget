#!/usr/bin/env python

import cv2
import numpy as np
import sys, getopt

def main(argv):
    if (len(argv) < 3):
        print 'usage: ' + sys.argv[0] + ' imagefile angle outfile'
        sys.exit(1)

    imagefile = argv[0]
    angle = int(argv[1])
    outfile = argv[2]
    img = cv2.imread(imagefile)
    rows, cols = img.shape[:2]
    print rows, cols
    center = ((cols-1.0)/2, (rows-1.0)/2)
    matrix = cv2.getRotationMatrix2D(center,angle,1)
    dst = cv2.warpAffine(img,matrix,(cols,rows))

    cv2.imwrite(outfile, dst)

if __name__ == "__main__":
    main(sys.argv[1:])
