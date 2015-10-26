import os
import sys
import shutil

for line in sys.stdin:
    paths = line.strip().split(" ")
    dstdir = os.path.dirname(paths[1])
    if not os.path.isdir(dstdir):
        os.makedirs(dstdir)
    shutil.copy(paths[0], paths[1])
