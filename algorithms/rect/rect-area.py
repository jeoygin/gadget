#!/usr/bin/env python

import os, sys
from rects import Rect, rect_area, load_rect_from_file, load_rect_from_path

def main(argv):
    if len(argv) > 0:
        checkfile(argv[0])
        rects = load_rect_from_path(argv[0])
    else:
        rects = load_rect_from_file(sys.stdin)

    print rect_area(rects)
    
def checkfile(path):
    if not os.path.isfile(path):
        print 'No such file: {0}'.format(path)
        sys.exit(1)
        
if __name__ == "__main__":
    main(sys.argv[1:])
