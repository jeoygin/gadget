import os
import sys

processed = 0

for line in sys.stdin:
    paths = line.strip().split(" ")
    try:
        os.renames(paths[0], paths[1])
    except OSError:
        print >> sys.stderr, paths[0] + " " + paths[1]
  
    processed = processed + 1
    if processed % 1000 == 0:
        print 'Processed {} files.'.format(processed)

if processed % 1000 > 0:
    print 'Processed {} files.'.format(processed)
