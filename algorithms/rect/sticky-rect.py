#!/usr/bin/env python

import os, sys, argparse
from rects import Rect, load_rect_from_path

def main(args):
    checkfile(args.targetfile)
    checkfile(args.stickyfile)

    target_list = load_rect_from_path(args.targetfile)
    sticky_list = load_rect_from_path(args.stickyfile)

    for rect in target_list:
        y0 = rect.y
        y1 = rect.y + rect.height
        top = y0 - 10
        bottom = y1 + 10
        for sticky_rect in sticky_list:
            intersect = rect.intersect(sticky_rect)
            if intersect.width > 0 and intersect.height > 0:
                if abs(sticky_rect.y - y0) < abs(top - y0):
                    top = sticky_rect.y
                if abs((sticky_rect.y + sticky_rect.height) - y1) < abs(bottom - y1):
                    bottom = sticky_rect.y + sticky_rect.height
        if abs(top - y0) < 10:
            y0 = top
        if abs(bottom - y1) < 10:
            y1 = bottom
        rect.y = y0
        rect.height = y1 - y0
        print rect
        
def checkdir(path):
    dir = os.path.dirname(path)
    if dir and not os.path.isdir(dir):
        os.makedirs(dir)

def checkfile(path):
    if not os.path.isfile(path):
        print 'No such file: ' + path
        sys.exit(1)

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description='Absorb horizontal line to the rectangles in the second file')
    parser.add_argument('targetfile', metavar="TARGETFILE",
                        help='target file')
    parser.add_argument('stickyfile', metavar="STICKYFILE",
                        help='sticky file')
    args = parser.parse_args()
    main(args)
