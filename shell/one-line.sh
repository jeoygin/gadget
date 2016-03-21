# Change date format
date -j -f "%Y%m%d%H%M%S" "20160303010203" +"%s"

# Calculate average value
awk '{ sum += $1; n++ } END { if (n > 0) print sum / n; }' FILE

# Extract commits between revisions R1 and R2, and apply them on top of the current branch
git format-patch -k --stdout R1..R2 | git am -3 -k

# Download tarball and extract
$ curl http://www.example.com/test.tar.gz | tar zx

