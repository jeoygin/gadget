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

void doit(const char * from, const char * to) {
  char buf[512];
  struct stat st = {0};
  strncpy(buf, to, sizeof(buf));
  char * dir = dirname(buf);
  makedirs(dir, 0775);

  cv::Mat img = cv::imread(from, CV_LOAD_IMAGE_GRAYSCALE);
  cv::imwrite(to, img);
}

int main(int argc, char** argv) {
  if (argc >= 3) {
    doit(argv[1], argv[2]);
  } else {
    string srcpath, dstpath;
    int processed = 0;
    while (cin >> srcpath >> dstpath) {
      doit(srcpath.c_str(), dstpath.c_str());
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
