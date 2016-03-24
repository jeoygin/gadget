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

Mat equalize(const Mat& src) {
    int min = 255, max = 0, val;
    Mat dst(src);
    vector<int> vec;
    for (int x = 0; x < src.cols; x++) {
        for (int y = 0; y < src.rows; y++) {
            val = src.ptr<uchar>(y)[x];
            vec.push_back(val);
        }
    }
    sort(vec.begin(), vec.end());
    min = vec[vec.size() * 0.05];
    max = vec[vec.size() * 0.95];
    for (int x = 0; x < src.cols; x++) {
        for (int y = 0; y < src.rows; y++) {
            val = src.ptr<uchar>(y)[x];
            val = val < min ? min : val > max ? max : val;
            int rval = (int)(((val - min) / (float)(max - min + 1)) * 255);
            dst.ptr<uchar>(y)[x] = rval;
        }
    }
    return dst;
}

int main(int argc, char** argv) {
  char buf[512];
  if (argc >= 3) {
    struct stat st = {0};
    strncpy(buf, argv[2], sizeof(buf));
    char * dir = dirname(buf);
    makedirs(dir, 0775);
    cout << argv[1] << endl;
    Mat src = imread(argv[1], CV_LOAD_IMAGE_UNCHANGED);
    Mat dst = equalize(src);
    imwrite(argv[2], dst);
  } else {
    string srcpath, dstpath;
    int processed = 0;
    while (cin >> srcpath >> dstpath) {
      struct stat st = {0};
      strncpy(buf, dstpath.c_str(), sizeof(buf));
      char * dir = dirname(buf);
      makedirs(dir, 0775);

      Mat src = imread(srcpath, CV_LOAD_IMAGE_UNCHANGED);
      Mat dst = equalize(src);
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
