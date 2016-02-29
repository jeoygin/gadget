#!/usr/bin/env python

import os, sys

class Rect:
    def __init__(self, x, y, width, height):
        self.x = x
        self.y = y
        self.width = width
        self.height = height

    def __str__(self):
        return '{0} {1} {2} {3}'.format(self.x, self.y, self.width, self.height)
        
    def intersect(self, that):
        x0 = max(self.x, that.x)
        x1 = min(self.x + self.width, that.x + that.width)
        y0 = max(self.y, that.y)
        y1 = min(self.y + self.height, that.y + that.height)
        if x0 < x1 and y0 < y1:
            return Rect(x0, y0, x1 - x0, y1 - y0)
        else:
            return Rect(0, 0, 0, 0)


def main(argv):
    if len(argv) < 2:
        print 'Usage: {0} file1 file2'.format(sys.argv[0])
        sys.exit(1)

    checkfile(argv[0]);
    checkfile(argv[1]);

    rect_list_one = load_rect(argv[0])
    rect_list_second = load_rect(argv[1])

    for rect1 in rect_list_one:
        for rect2 in rect_list_second:
            intersect = rect1.intersect(rect2)
            if intersect.width > 0 and intersect.height > 0:
                print intersect
    
def load_rect(file_path):
    rects = []
    with open(file_path) as f:
        for line in f:
            x, y, width, height = [int(i) for i in line.split()]
            rects.append(Rect(x, y, width, height))
    return rects
    
def checkfile(path):
    if not os.path.isfile(path):
        print 'No such file: {0}'.format(path)
        sys.exit(1)

if __name__ == "__main__":
    main(sys.argv[1:])
