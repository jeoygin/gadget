from sets import Set
import heapq

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

def rect_area(rects):
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

    return area
        
def load_rect_from_file(file):
    rects = []
    for line in file:
        x, y, width, height = [int(i) for i in line.split()]
        rects.append(Rect(x, y, width, height))
    return rects        

def load_rect_from_path(path):
    with open(path) as file:
        return load_rect_from_file(file)
