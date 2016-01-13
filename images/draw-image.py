#!/usr/bin/env python

import cv2
import os, sys, getopt

def main(argv):
    if len(argv) < 2:
        print 'Usage: ' + sys.argv[0] + ' imagefile outfile [drawfile]'
        sys.exit(1)

    checkfile(argv[0])
    if len(argv) > 2:
        checkfile(argv[2])

    imagefile = argv[0]
    outfile = argv[1]
    drawfile = open(argv[2]) if len(argv) > 2 else sys.stdin

    img = cv2.imread(imagefile)
    line = drawfile.readline()
    while line:
        params = line.split()
        if len(params) > 0:
            draw(img, params)
        line = drawfile.readline()

    checkdir(outfile)
    cv2.imwrite(outfile, img)

def draw(img, params):
    if params[0] == 'rect':
        drawrect(img, params)
        
def drawrect(img, params):
    if len(params) >= 5:
        x = int(params[1])
        y = int(params[2])
        width = int(params[3])
        height = int(params[4])
        cv2.rectangle(img, (x, y), (x + width, y + height), (0, 128, 255), 3)
        
def checkdir(path):
    dir = os.path.dirname(path)
    if not os.path.isdir(dir):
        os.makedirs(dir)

def checkfile(path):
    if not os.path.isfile(path):
        print 'No such file: ' + path
        sys.exit(1)

if __name__ == "__main__":
    main(sys.argv[1:])
