import os
import sys
import shutil

processed = 0

for line in sys.stdin:
    paths = line.strip().split(" ")
    dstdir = os.path.dirname(paths[1])
    if not os.path.isdir(dstdir):
        os.makedirs(dstdir)
    shutil.copy(paths[0], paths[1])
    processed = processed + 1
    if processed % 1000 == 0:
        print 'Processed {} files.'.format(processed)

if processed % 1000 > 0:
    print 'Processed {} files.'.format(processed)
