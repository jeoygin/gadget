import os
import sys

for line in sys.stdin:
    paths = line.strip().split(" ")
    try:
        os.renames(paths[0], paths[1])
    except OSError:
        print >> sys.stderr, paths[0] + " " + paths[1]
