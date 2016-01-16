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

cv::Mat resize(cv::Mat src, int maxsize) {
  cv::Mat dst;
  double scale = 1.0 * maxsize / max(src.rows, src.cols);
  cv::resize(src, dst, cv::Size((int)(src.cols * scale), (int)(src.rows * scale)), CV_INTER_CUBIC);
  return dst;
}

int main(int argc, char** argv) {
  char buf[512];
  if (argc >= 4) {
    struct stat st = {0};
    strncpy(buf, argv[2], sizeof(buf));
    char * dir = dirname(buf);
    makedirs(dir, 0775);

    cv::Mat src = cv::imread(argv[1], CV_LOAD_IMAGE_UNCHANGED);
    cv::Mat dst = resize(src, atoi(argv[3]));
    cv::imwrite(argv[2], dst);
  } else {
    string srcpath, dstpath;
    int maxsize;
    int processed = 0;
    while (cin >> srcpath >> dstpath >> maxsize) {
      struct stat st = {0};
      strncpy(buf, dstpath.c_str(), sizeof(buf));
      char * dir = dirname(buf);
      makedirs(dir, 0775);

      cv::Mat src = cv::imread(srcpath, CV_LOAD_IMAGE_UNCHANGED);
      cv::Mat dst = resize(src, maxsize);
      cv::imwrite(dstpath, dst);
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
