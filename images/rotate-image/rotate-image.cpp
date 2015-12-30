#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <sys/types.h>
#include <sys/stat.h>
#include <libgen.h>
#include <iostream>

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

Mat rotate(Mat src, double angle) {
  Mat dst;
  Point2f center((src.cols-1)/2., (src.rows-1)/2.);
  Mat r = getRotationMatrix2D(center, angle, 1.0);
  warpAffine(src, dst, r, Size(src.cols, src.rows));
  return dst;
}

int main(int argc, char** argv) {
  char buf[512];
  if (argc >= 4) {
    struct stat st = {0};
    strncpy(buf, argv[2], sizeof(buf));
    char * dir = dirname(buf);
    makedirs(dir, 0775);
    Mat src = imread(argv[1]);
    Mat dst = rotate(src, atoi(argv[3]));
    imwrite(argv[2], dst);
  } else {
    string srcpath, dstpath;
    int angle;
    int processed = 0;
    while (cin >> srcpath >> dstpath >> angle) {
      struct stat st = {0};
      strncpy(buf, dstpath.c_str(), sizeof(buf));
      char * dir = dirname(buf);
      makedirs(dir, 0775);
      Mat src = imread(srcpath);
      Mat dst = rotate(src, angle);
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
