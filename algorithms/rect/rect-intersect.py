#!/usr/bin/env python

import os, sys, argparse
from rects import Rect, rect_area, load_rect_from_path

def main(args):
    checkfile(args.file0);
    checkfile(args.file1);

    rect_list_one = load_rect_from_path(args.file0)
    rect_list_second = load_rect_from_path(args.file1)

    intersections = []
    for rect1 in rect_list_one:
        for rect2 in rect_list_second:
            rect = rect1.intersect(rect2)
            if rect.width > 0 and rect.height > 0:
                intersections.append(rect)

    if args.print_area:
        print('%d %d %d' % (rect_area(rect_list_one),
                            rect_area(rect_list_second),
                            rect_area(intersections)))
    else:
        for i in intersections:
            print i
    
def checkfile(path):
    if not os.path.isfile(path):
        print 'No such file: {0}'.format(path)
        sys.exit(1)

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description='Calculate intersection of two collections of rects')
    parser.add_argument('file0', metavar="FILE0",
                        help='file0')
    parser.add_argument('file1', metavar="FILE1",
                        help='file1')
    parser.add_argument('-a', '--area', action='store_true', dest='print_area',
                        help='print intersection area instead of rects')
    args = parser.parse_args()
    main(args)
