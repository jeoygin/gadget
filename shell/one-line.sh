#
# Common
#

## Replace foo in last command with bar
$ ^foo^bar

## Create backup file
$ cp filename{,.bak}

#
# Xargs
#

$ awk '{print $1,$2}' FILE | xargs -n 2 bash -c 'SHELL $0 + $1'

#
# Date
#

## Change date format

### Mac
date -j -f "%Y%m%d%H%M%S" "20160303010203" +"%s"

### Linux
date --date "2016-03-03T01:02:03"  +"%s"
date --date "@1456938123"  +"%Y%m%d%H%M%S"

#
# Awk
#

## Calculate average value
awk '{ sum += $1; n++ } END { if (n > 0) print sum / n; }' FILE

## Group by
awk '{freq[$1]++} END {for (f in freq) print f, freq[f]}' FILE

## Pring path dirname and basename
echo "/usr/local/bin"  | awk '{match($1, "[^/]+$"); print substr($1, 0, RSTART-1), substr($1, RSTART, RLENGTH)}'
echo "/usr/local/bin"  | awk '{split($1, parts ,"/"); print parts[length(parts)]}'

#
# Git
#

## Extract commits between revisions R1 and R2, and apply them on top of the current branch
git format-patch -k --stdout R1..R2 | git am -3 -k

#
# Data Download & Upload & Sync
#

## Download tarball and extract
$ curl http://www.example.com/test.tar.gz | tar zx

## Download and write to file
$ curl -sL -o /path/to/local/test.tar.gz http://www.example.com/test.tar.gz

## rsync
$ rsync -avP source destination

#
# List Listening Network Port
#

## Max OSX
$ netstat -anp tcp | grep -i 'listen'
$ lsof -iTCP -nP | grep -i 'listen'
$ lsof -iTCP -sTCP:LISTEN -nP
$ lsof -i :$port -nP

## Linux
$ netstat -ntlp

#
# HTTP Static Server
#

$ python -m SimpleHTTPServer 8000 # python 2
$ python -m http.server 8000 # python 3
$ npm install -g http-server && http-server -p 8000 # Node.js
$ curl https://gist.github.com/willurd/5720255 # for more other languages

#
# Linux Distribution Package Manager
#

## CentOS Package Manager
$ yum list installed
$ rpm -ql packageName

#
# Dynamic Linker Run-time Bindings
#

## Rebuild the cache

$ ldconfig

## Only process the specified directories

$ ldconfig -n /opt/dummy/lib

## Display current libraries from the cache

$ ldconfig -p

## Display libraries from every directory

$ ldconfig -v

