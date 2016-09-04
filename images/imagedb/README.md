# Image DB

## How to Build ##

```
$ git submodule update --init
$ mkdir build
$ cd build
$ cmake ..
$ cmake --build .
$ bin/imagedb
Usage: imagedb [options]

Options:
  --help                print help messages
  -c [ --cmd ] arg      command
  -s [ --src ] arg      src path
  -d [ --dst ] arg      dst path
  -l [ --list ] arg     image list file path
  -o [ --ops ] arg      operations config file path
  -v [ --verbose ]      produce verbose output
```

## Examples ##

### List all keys in DB ###

```
$ bin/imagedb -s /path/to/local/dir -c list
```

```
$ bin/imagedb -s leveldb:/path/to/leveldb/dir -c list
```

### Display images ###

```
$ echo "test.png" | bin/imagedb -s /path/to/local/dir -c show
```

```
$ bin/imagedb -s /path/to/local/dir -c show -l img.lst
$ cat img.lst
test.png
```

### Pipe Operations ###

```
$ bin/imagedb -s /path/to/local/dir -c pipe -l img.lst -o ops.yaml
$ cat img.lst
test.png::test_10_15_50_80.jpeg::10::15::50::80
$ cat ops.yaml
- cmd: sizeup
  size: 1024
- cmd: clip
  x: $3
  y: $4
  w: $5
  h: $6
- cmd: gray
- cmd: invert
- cmd: equ
- cmd: save
  db: /path/to/dst/dir
  key: $2
- cmd: show
```
