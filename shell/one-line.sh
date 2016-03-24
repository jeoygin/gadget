# Change date format
date -j -f "%Y%m%d%H%M%S" "20160303010203" +"%s"

# Calculate average value
awk '{ sum += $1; n++ } END { if (n > 0) print sum / n; }' FILE

# Extract commits between revisions R1 and R2, and apply them on top of the current branch
git format-patch -k --stdout R1..R2 | git am -3 -k

# Download tarball and extract
$ curl http://www.example.com/test.tar.gz | tar zx

# rsync
$ rsync -avP source destination

# Download and write to file
$ curl -sL -o /path/to/local/test.tar.gz http://www.example.com/test.tar.gz

# Start HTTP Static Server
$ python -m SimpleHTTPServer 8000 # python 2
$ python -m http.server 8000 # python 3
$ npm install -g http-server && http-server -p 8000 # Node.js
$ curl https://gist.github.com/willurd/5720255 # for more other languages

# Replace foo in last command with bar
$ ^foo^bar

# Create backup file
$ cp filename{,.bak}


