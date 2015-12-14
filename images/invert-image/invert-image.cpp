#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <sys/types.h>
#include <sys/stat.h>
#include <libgen.h>
#include <iostream>

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

cv::Mat invert(cv::Mat src) {
  cv::Mat dst = (255 - src);
  return dst;
}

int main(int argc, char** argv) {
  char buf[512];
  if (argc >= 3) {
    struct stat st = {0};
    strncpy(buf, argv[2], sizeof(buf));
    char * dir = dirname(buf);
    makedirs(dir, 0775);

    cv::Mat src = cv::imread(argv[1], CV_LOAD_IMAGE_UNCHANGED);
    cv::Mat dst = invert(src);
    cv::imwrite(argv[2], dst);
  } else {
    string srcpath, dstpath;
    while (cin >> srcpath >> dstpath) {
      struct stat st = {0};
      strncpy(buf, dstpath.c_str(), sizeof(buf));
      char * dir = dirname(buf);
      makedirs(dir, 0775);

      cv::Mat src = cv::imread(srcpath, CV_LOAD_IMAGE_UNCHANGED);
      cv::Mat dst = invert(src);
      cv::imwrite(dstpath, dst);
    }
  }

  return 0;
}
