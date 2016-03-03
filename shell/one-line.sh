# Change date format
date -j -f "%Y%m%d%H%M%S" "20160303010203" +"%s"

# Calculate average value
awk '{ sum += $1; n++ } END { if (n > 0) print sum / n; }' FILE
