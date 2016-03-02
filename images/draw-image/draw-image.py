#!/usr/bin/env python

import cv2
import os, sys, argparse

def main(args):
    checkfile(args.imagefile)
    if args.shapefile:
        checkfile(args.shapefile)

    imagefile = args.imagefile
    outfile = args.outfile
    shapefile = open(args.shapefile) if args.shapefile else sys.stdin

    img = cv2.imread(imagefile)
    line = shapefile.readline()
    while line:
        params = line.split()
        if len(params) > 0:
            draw(img, params, thickness=args.thickness,
                 red=args.red, green=args.green, blue=args.blue)
        line = shapefile.readline()

    checkdir(outfile)
    cv2.imwrite(outfile, img)

def draw(img, params, **config):
    if params[0] == 'rect':
        drawrect(img, params, **config)
        
def drawrect(img, params, **config):
    if len(params) >= 5:
        x = int(params[1])
        y = int(params[2])
        width = int(params[3])
        height = int(params[4])
        c = int(params[5]) if len(params) >= 6 else 3
        r = config['red'] if c & 1 > 0 else 0
        g = config['green'] if c & 2 > 0 else 0
        b = config['blue'] if c & 4 > 0 else 0
        cv2.rectangle(img, (x, y), (x + width, y + height), (b, g, r), config['thickness'])
        
def checkdir(path):
    dir = os.path.dirname(path)
    if dir and not os.path.isdir(dir):
        os.makedirs(dir)

def checkfile(path):
    if not os.path.isfile(path):
        print 'No such file: ' + path
        sys.exit(1)

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description='Draw some shapes on an image')
    parser.add_argument('imagefile', metavar="IMAGEFILE",
                        help='input image file')
    parser.add_argument('outfile', metavar="OUTFILE",
                        help='output image file')
    parser.add_argument('-s', '--shape', metavar='SHAPEFILE', dest='shapefile',
                        help='file storing shapes, one line for one shape');
    parser.add_argument('-t', '--thickness', metavar='THICKNESS', type=int, default=1,
                        help='shape border thickness')
    parser.add_argument('-r', '--red', metavar='RED', type=int, default=192,
                        help='R level of color')
    parser.add_argument('-g', '--green', metavar='GREEN', type=int, default=192,
                        help='G level of color')
    parser.add_argument('-b', '--blue', metavar='BLUE', type=int, default=192,
                        help='B level of color')
    args = parser.parse_args()
    main(args)
