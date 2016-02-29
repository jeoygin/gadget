#!/usr/bin/env python

import os, sys
import heapq
from sets import Set

class Rect:
    def __init__(self, x, y, width, height):
        self.x = x
        self.y = y
        self.width = width
        self.height = height

    def __str__(self):
        return '{0} {1} {2} {3}'.format(self.x, self.y, self.width, self.height)

def main(argv):
    if len(argv) > 0:
        checkfile(argv[0])
        with open(argv[0]) as f:
            rects = load_rect(f)
    else:
        rects = load_rect(sys.stdin)

    queue = sorted(rects, key=lambda o:o.x)
        
    xset = Set()
    for rect in rects:
        xset.add(rect.x)
        xset.add(rect.x + rect.width)

    heap = []
    lastx = 0
    area = 0
    for x in sorted(xset):
        ylist = []
        for t in heap:
            ylist.append((t[1].y, 0))
            ylist.append((t[1].y + t[1].height, 1))
        ylist.sort()
        ystart = 0
        nstart = 0
        for t in ylist:
            if t[1] == 0:
                if nstart == 0:
                    ystart = t[0]
                nstart += 1
            else:
                nstart -= 1
                if nstart == 0:
                     area += (x - lastx) * (t[0] - ystart)
                     
        while len(heap) > 0 and heap[0][0] <= x:
            heapq.heappop(heap)

        while len(queue) > 0 and queue[0].x == x:
            cur = queue.pop(0)
            heapq.heappush(heap, (cur.x + cur.width, cur))

        lastx = x

    print area
    
def checkfile(path):
    if not os.path.isfile(path):
        print 'No such file: {0}'.format(path)
        sys.exit(1)

def load_rect(file):
    rects = []
    for line in file:
        x, y, width, height = [int(i) for i in line.split()]
        rects.append(Rect(x, y, width, height))
    return rects        
        
if __name__ == "__main__":
    main(sys.argv[1:])
