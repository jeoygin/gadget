#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <sys/types.h>
#include <sys/stat.h>
#include <libgen.h>
#include <iostream>
#include "binary-img.h"

using namespace cv;
using namespace std;

int makedirs(char * path, mode_t mode) {
    struct stat st = {0};

    if (stat(path, &st) == 0) {
        if (S_ISDIR(st.st_mode) == 0) {
            return -1;
        }
        return 0;
    }

    char subpath[512] = "";
    char * delim = strrchr(path, '/');
    if (delim != NULL) {
        strncat(subpath, path, delim - path);
        makedirs(subpath, mode);
    }
    if (mkdir(path, mode) != 0) {
        return -1;
    }
    return 0;
}

Mat doit(Mat src) {
  Mat gray, dst;
  cvtColor( src, gray, CV_BGR2GRAY );
  Mat invert = (255 - gray);
  binary_v5::Img_Binary( invert, dst );
  // threshold( src, dst, 80, 255, 0 );
  return (255 - dst);
}

int main(int argc, char** argv) {
  char buf[512];
  if (argc >= 3) {
    struct stat st = {0};
    strncpy(buf, argv[2], sizeof(buf));
    char * dir = dirname(buf);
    makedirs(dir, 0775);
    Mat src = imread(argv[1], 0);
    Mat dst = doit(src);
    imwrite(argv[2], dst);
  } else {
    string srcpath, dstpath;
    int processed = 0;
    while (cin >> srcpath >> dstpath) {
      struct stat st = {0};
      strncpy(buf, dstpath.c_str(), sizeof(buf));
      char * dir = dirname(buf);
      makedirs(dir, 0775);
      Mat src = imread(srcpath, 1);
      Mat dst = doit(src);
      imwrite(dstpath, dst);
      ++processed;
      if (processed % 1000 == 0) {
        cout << "Processed " << processed << " files." << endl;
      }
    }
    if (processed % 1000 != 0) {
      cout << "Processed " << processed << " files." << endl;
    }
  }

  return 0;
}
